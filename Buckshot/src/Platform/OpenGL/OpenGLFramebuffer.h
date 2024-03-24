#pragma once

#include "Buckshot/Renderer/Framebuffer.h"

namespace Buckshot {

  class OpenGLFramebuffer : public Framebuffer
  {
  public:
    OpenGLFramebuffer(const FramebufferSpecification& specification);
    virtual ~OpenGLFramebuffer() override;

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual int ReadPixel(uint32_t attachment_index, int x, int y) override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { BS_ASSERT(index < m_ColorAttachments.size(), "Invalid index specified"); return m_ColorAttachments[index]; }

    void Invalidate();

  private:
    uint32_t m_RendererID = 0;

    FramebufferSpecification m_Specification;
    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
  };

}