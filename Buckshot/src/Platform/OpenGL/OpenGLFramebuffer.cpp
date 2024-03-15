#include <bspch.h>
#include <GLAD/glad.h>

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Buckshot {

  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
  {
    BS_PROFILE_FUNCTION();

    m_Specification = specification;
    Invalidate();
  }

  OpenGLFramebuffer::~OpenGLFramebuffer()
  {
    BS_PROFILE_FUNCTION();

    glDeleteFramebuffers(1, &m_RendererID);
  }

  void OpenGLFramebuffer::Bind()
  {
    BS_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
  }

  void OpenGLFramebuffer::Unbind()
  {
    BS_PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFramebuffer::Invalidate()
  {
    BS_PROFILE_FUNCTION();

    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

    BS_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

}