#include <bspch.h>
#include "Buckshot/Renderer/GraphicsContext.h"

#include "Buckshot/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Buckshot {

  Scope<GraphicsContext> GraphicsContext::Create(void* window)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLContext>(window);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}