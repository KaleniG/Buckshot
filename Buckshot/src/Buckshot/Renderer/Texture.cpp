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
      BS_ASSERT(false, "RendererAPI::None is not supported")
        return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI")
        return nullptr;
    }
  }

}