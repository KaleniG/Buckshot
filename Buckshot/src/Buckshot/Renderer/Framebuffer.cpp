#include <bspch.h>

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Buckshot/Renderer/Framebuffer.h"
#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {

  Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLFramebuffer>(specification);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}