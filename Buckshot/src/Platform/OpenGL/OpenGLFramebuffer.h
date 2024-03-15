#pragma once

#include "Buckshot/Renderer/Framebuffer.h"

namespace Buckshot {

  class OpenGLFramebuffer : public Framebuffer
  {
  public:
    OpenGLFramebuffer(const FramebufferSpecification& specification);
    virtual ~OpenGLFramebuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

    void Invalidate();

  private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment;
    uint32_t m_DepthAttachment;
    FramebufferSpecification m_Specification;
  };

}