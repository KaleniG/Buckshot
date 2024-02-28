#pragma once

#include <glm/glm.hpp>

#include "Buckshot/Renderer/Shader.h"

typedef unsigned int GLenum; // TO REMOVE

namespace Buckshot {

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource);
    OpenGLShader(const std::string& filepath);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    virtual void SetMat4(const std::string& name, const glm::mat4& data) override;
    virtual void SetFloat3(const std::string& name, const glm::vec3& data) override;
    virtual void SetFloat4(const std::string& name, const glm::vec4& data) override;

    virtual const std::string& GetName() const override { return m_Name; }

    void UploadUniformMat4(const std::string& name, const glm::mat4& data);
    void UploadUniformMat3(const std::string& name, const glm::mat3& data);
    void UploadUniformInt(const std::string& name, int data);
    void UploadUniformFloat(const std::string& name, float data);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& data);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& data);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& data);

  private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& shaderSource);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources );

  private:
    uint32_t m_RendererID;
    std::string m_Name;
  };

}