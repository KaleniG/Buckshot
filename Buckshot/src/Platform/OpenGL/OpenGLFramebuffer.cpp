#include <bspch.h>
#include <GLAD/glad.h>

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Buckshot {

  static const uint32_t s_MaxFramebufferSize = 8192;

  namespace Utilities {
    static GLenum FrambufferTextureFormat_BuckshotToOpenGL(FramebufferTextureFormat format)
    {
      switch (format)
      {
      case Buckshot::FramebufferTextureFormat::RGBA8:
        return GL_RGBA8;
      case Buckshot::FramebufferTextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH24_STENCIL8;
      case Buckshot::FramebufferTextureFormat::RED_INT:
        return GL_RED_INTEGER;
      }
      BS_ASSERT(false, "Invalid Framebuffer Texture Format specified, enum = \"{0}\"", (int)format);
      return GL_INVALID_ENUM;
    }

    static GLenum TextureTarget(bool is_multisampled)
    {
      return is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool is_multisampled, uint32_t* out_id, uint32_t count)
    {
      glCreateTextures(TextureTarget(is_multisampled), count, out_id);

    }

    static void BindTexture(bool is_multisampled, uint32_t id)
    {
      glBindTexture(TextureTarget(is_multisampled), id);
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
      switch (format)
      {
      case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
      }
      return false;
    }

    static void AttachColorTexture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
    {
      bool multisampled = samples > 1;

      if (multisampled)
      {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
      }
      else
      {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height)
    {
      bool multisampled = samples > 1;

      if (multisampled)
      {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
      }
      else
      {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, TextureTarget(multisampled), id, 0);
    }
  }

  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
  {
    BS_PROFILE_FUNCTION();

    m_Specification = specification;

    for (auto spec : m_Specification.Attachments.Attachments)
    {
      if (!Utilities::IsDepthFormat(spec.TextureFormat))
        m_ColorAttachmentSpecifications.emplace_back(spec);
      else
        m_DepthAttachmentSpecification = spec;
    }

    Invalidate();
  }

  OpenGLFramebuffer::~OpenGLFramebuffer()
  {
    BS_PROFILE_FUNCTION();

    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
  }

  void OpenGLFramebuffer::Bind()
  {
    BS_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
  }

  void OpenGLFramebuffer::Unbind()
  {
    BS_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFramebuffer::ClearAttachment(uint32_t attachment_index, int value)
  {
    BS_ASSERT(attachment_index < m_ColorAttachments.size(), "Invalid index, only up to 4 attachments can be done");

    auto spec = m_ColorAttachmentSpecifications[attachment_index];
    glClearTexImage(m_ColorAttachments[attachment_index], 0, Utilities::FrambufferTextureFormat_BuckshotToOpenGL(spec.TextureFormat), GL_INT, &value);
  }

  int OpenGLFramebuffer::ReadPixel(uint32_t attachment_index, int x, int y)
  {
    BS_ASSERT(attachment_index < m_ColorAttachments.size(), "Invalid index, only up to 4 attachments can be done");

    int pixel_data;

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

    return pixel_data;
  }

  void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
  {
    BS_PROFILE_FUNCTION();

    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
      BS_WARN("Attempted to resize the framebuffer to {0}, {1}", width, height);
      return;
    }

    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
  }

  void OpenGLFramebuffer::Invalidate()
  {
    BS_PROFILE_FUNCTION();

    if (m_RendererID)
    {
      glDeleteFramebuffers(1, &m_RendererID);
      glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
      glDeleteTextures(1, &m_DepthAttachment);

      m_ColorAttachments.clear();
      m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    bool multisample = m_Specification.Samples > 1;

    if (m_ColorAttachmentSpecifications.size())
    {
      m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
      Utilities::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
      
      for (size_t i = 0; i < m_ColorAttachments.size(); i++)
      {
        Utilities::BindTexture(multisample, m_ColorAttachments[i]);

        switch (m_ColorAttachmentSpecifications[i].TextureFormat)
        {
          case FramebufferTextureFormat::RGBA8:
            Utilities::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
            break;
          case FramebufferTextureFormat::RED_INT:
            Utilities::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
            break;
        }
      }
    }

    if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
    {
      Utilities::CreateTextures(multisample, &m_DepthAttachment, 1);
      Utilities::BindTexture(multisample, m_DepthAttachment);

      switch (m_DepthAttachmentSpecification.TextureFormat)
      {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          Utilities::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
          break;
      }
    }

    if (m_ColorAttachments.size() > 1)
    {
      BS_ASSERT(m_ColorAttachments.size() <= 4, "Only support up to 4 color attachments");

      GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
      glDrawBuffers(m_ColorAttachments.size(), buffers);
    }
    else if (m_ColorAttachments.empty())
    {
      glDrawBuffer(GL_NONE);
    }

    BS_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

}