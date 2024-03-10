#include <bspch.h>

#include "Buckshot/Renderer/Buffers.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Buckshot {

  Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexBuffer>(size);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

  Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexBuffer>(vertices, size);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t count)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLIndexBuffer>(indicies, count);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}