#include <bspch.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "VertexArray.h"
#include "Renderer.h"

namespace Buckshot {

  VertexArray* VertexArray::Create()
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::None:
      BS_ASSERT(false, "RendererAPI::None is not supported")
      return nullptr;
      break;
    case RendererAPI::OpenGL:
      return new OpenGLVertexArray();
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI")
      return nullptr;
    }
  }

}