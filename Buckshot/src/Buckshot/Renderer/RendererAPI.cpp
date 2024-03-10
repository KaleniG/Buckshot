#include <bspch.h>

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  RendererAPI::API RendererAPI::m_API = API::OpenGL;

  Scope<RendererAPI> RendererAPI::Create()
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLRendererAPI>();
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}