#pragma once

#include <glm/glm.hpp>

namespace Buckshot
{

  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetMat4(const std::string& name, const glm::mat4& data) = 0;
    virtual void SetFloat(const std::string& name, float data) = 0;
    virtual void SetFloat2(const std::string& name, const glm::vec2& data) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& data) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& data) = 0;
    virtual void SetInt(const std::string& name, int data) = 0;
    virtual void SetIntArray(const std::string& name, int* data, uint32_t count) = 0;

    virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::string& filepath);
    static Ref<Shader> Create(const std::string& name, const std::string& vertSource, const std::string& fragSource);
  };

  class ShaderLibrary
  {
  public:
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name, const Ref<Shader>& shader);
    
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name) const;

  private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
  };
}