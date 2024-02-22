#include <bspch.h>

#include "Buffers.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Buckshot {

  VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::None:
      BS_ASSERT(false, "RendererAPI::None is not supported")
      return nullptr;
      break;
    case RendererAPI::OpenGL:
      return new OpenGLVertexBuffer(vertices, size);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI")
      return nullptr;
    }
  }

  IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t count)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::None:
      BS_ASSERT(false, "RendererAPI::None is not supported")
        return nullptr;
      break;
    case RendererAPI::OpenGL:
      return new OpenGLIndexBuffer(indicies, count);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI")
        return nullptr;
    }
  }

}