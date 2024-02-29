#include <bspch.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {

  Ref<VertexArray> VertexArray::Create()
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexArray>();
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}