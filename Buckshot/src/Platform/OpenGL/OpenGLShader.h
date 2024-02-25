#pragma once

#include <glm/glm.hpp>

#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::string& vertSource, const std::string& fragSource);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void UploadUniformMat4(const std::string& name, const glm::mat4& data);
    void UploadUniformMat3(const std::string& name, const glm::mat3& data);
    void UploadUniformInt(const std::string& name, int data);
    void UploadUniformFloat(const std::string& name, float data);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& data);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& data);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& data);

  private:
    uint32_t m_RendererID;

  };

}