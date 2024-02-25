#include <bspch.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  Shader* Shader::Create(const std::string& vertSource, const std::string& fragSource)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported")
        return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertSource, fragSource);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI")
        return nullptr;
    }
  }

}