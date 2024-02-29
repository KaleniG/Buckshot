#include <bspch.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  Ref<Shader> Shader::Create(const std::string& filepath)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLShader>(filepath);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

  Ref<Shader> Shader::Create(const std::string& name, const std::string& vertSource, const std::string& fragSource)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
      BS_ASSERT(false, "RendererAPI::None is not supported");
      return nullptr;
      break;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLShader>(name, vertSource, fragSource);
      break;
    default:
      BS_ASSERT(false, "Unknown RendererAPI");
      return nullptr;
    }
  }

  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
    auto& name = shader->GetName();
    Add(name, shader);
  }

  void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
  {
    BS_ASSERT(!Exists(name), "Shader already exists");
    m_Shaders[name] = shader;
  }

  Ref<Buckshot::Shader> ShaderLibrary::Load(const std::string& filepath)
  {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
  }

  Ref<Buckshot::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
  {
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
  }

  Ref<Buckshot::Shader> ShaderLibrary::Get(const std::string& name)
  {
    BS_ASSERT(Exists(name), "Shader not found");
    return m_Shaders[name];
  }

  bool ShaderLibrary::Exists(const std::string& name) const
  {
    return m_Shaders.find(name) != m_Shaders.end();
  }

}