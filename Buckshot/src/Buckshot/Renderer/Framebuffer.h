#pragma once

#include "Buckshot/Core/Base.h"

namespace Buckshot {

  enum class FramebufferTextureFormat
  {
    None,

    // Color
    RGBA8,

    // Depth/Stencil
    DEPTH24STENCIL8,

    // Data
    RED_INT
  };

  struct FramebufferTextureSpecification
  {
    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
      : TextureFormat(format) {}

  };

  struct FramebufferAttachmentSpecification
  {
    std::vector<FramebufferTextureSpecification> Attachments;

    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> attachements)
      : Attachments(attachements) {}

  };

  struct FramebufferSpecification
  {
    FramebufferAttachmentSpecification Attachments;
    uint32_t Width = 0;
    uint32_t Height = 0;
    uint32_t Samples = 1;
    bool SwapChainTarget = false;
  };

  class Framebuffer
  {
  public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void ClearAttachment(uint32_t attachment_index, int value) = 0;

    virtual int ReadPixel(uint32_t attachment_index, int x, int y) = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual const FramebufferSpecification& GetSpecification() const = 0;
    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
  };

}