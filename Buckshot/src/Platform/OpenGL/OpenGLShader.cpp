#include <bspch.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.h"


namespace Buckshot {

  OpenGLShader::OpenGLShader(const std::string& vertSource, const std::string& fragSource)
  {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* source = vertSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      GLint logLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

      std::vector<GLchar> infoLog(logLength);
      glGetShaderInfoLog(vertexShader, logLength, &logLength, &infoLog[0]);

      glDeleteShader(vertexShader);

      BS_ERROR("Vertex Shader compilation failure");
      BS_ERROR("{0}", infoLog.data());
      return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    source = fragSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    glCompileShader(fragmentShader);

    isCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      GLint logLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

      std::vector<GLchar> infoLog(logLength);
      glGetShaderInfoLog(fragmentShader, logLength, &logLength, &infoLog[0]);

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      BS_ERROR("Fragment Shader compilation failure");
      BS_ERROR("{0}", infoLog.data());
      return;
    }

    m_RendererID = glCreateProgram();

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    glLinkProgram(m_RendererID);

    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
      GLint logLength = 0;
      glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &logLength);

      std::vector<GLchar> infoLog(logLength);
      glGetShaderInfoLog(m_RendererID, logLength, &logLength, &infoLog[0]);

      glDeleteProgram(m_RendererID);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      BS_ERROR("Shader linking failure");
      BS_ERROR("{0}", infoLog.data());
      return;
    }

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_RendererID);
  }

  void OpenGLShader::Bind() const
  {
    glUseProgram(m_RendererID);
  }

  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

  void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
  }

  void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
  }

  void OpenGLShader::UploadUniformInt(const std::string& name, int data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, data);
  }

  void OpenGLShader::UploadUniformFloat(const std::string& name, float data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, data);
  }

  void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, data.x, data.y);
  }

  void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, data.x, data.y, data.z);
  }

  void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& data)
  {
    uint32_t location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, data.x, data.y, data.z, data.w);
  }


}