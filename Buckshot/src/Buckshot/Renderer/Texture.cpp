#include <bspch.h>

#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Buckshot/Renderer/Texture.h"
#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {

  Ref<Texture2D> Texture2D::Create(const std::string& path)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLTexture2D>(path);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

  Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLTexture2D>(width, height);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

}