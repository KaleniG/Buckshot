#include <bspch.h>
#include <glad/glad.h>

#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  Shader::Shader(const std::string& vertSource, const std::string& fragSource)
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

  Shader::~Shader()
  {
    glDeleteProgram(m_RendererID);
  }

  void Shader::Bind() const
  {
    glUseProgram(m_RendererID);
  }

  void Shader::Unbind() const
  {
    glUseProgram(0);
  }

}