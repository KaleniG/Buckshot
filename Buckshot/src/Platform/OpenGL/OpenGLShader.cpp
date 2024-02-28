#include <bspch.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

namespace Buckshot {

  static GLenum ShaderTypeFromString(const std::string& type)
  {
    if (type == "vertex")
      return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
      return GL_FRAGMENT_SHADER;

    BS_ERROR("Specified unknown shader type \"{0}\" in the shader file", type);
    return 0;
  }

  OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSource, const std::string& fragSource)
  {
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = vertSource;
    shaderSources[GL_FRAGMENT_SHADER] = fragSource;
    Compile(shaderSources);

    m_Name = name;
  }

  OpenGLShader::OpenGLShader(const std::string& filepath)
  {
    std::string shader = ReadFile(filepath);
    auto shaderSources = PreProcess(shader);
    Compile(shaderSources);
    
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
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

  std::string OpenGLShader::ReadFile(const std::string& filepath)
  {
    std::string file_bin;
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (file)
    {
      file.seekg(0, std::ios::end);
      file_bin.resize(file.tellg());
      file.seekg(0, std::ios::beg);
      file.read(&file_bin[0], file_bin.size());
      file.close();
    }
    else
    {
      BS_ERROR("Error on loading external shader file \"{0}\"", filepath);
    }

    return file_bin;
  }

  std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& shaderSource)
  {
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = shaderSource.find(typeToken, 0);

    while (pos != std::string::npos)
    {
      size_t eol = shaderSource.find_first_of("\r\n", pos);
      BS_ASSERT(eol != std::string::npos, "SyntaxError in the shader file");
      size_t begin = pos + typeTokenLength + 1;
      std::string shaderType = shaderSource.substr(begin, eol - begin);
      BS_ASSERT(shaderType == "vertex" || shaderType == "fragment" || shaderType == "pixel", "Invalid shader type specified");

      size_t nextLinePos = shaderSource.find_first_not_of("\r\n", eol);
      pos = shaderSource.find(typeToken, nextLinePos);
      shaderSources[ShaderTypeFromString(shaderType)] = shaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
    }

    return shaderSources;
  }

  void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
  {
    BS_ASSERT(shaderSources.size() <= 2, "Only two types of shaders supported for now");
    GLuint program = glCreateProgram();
    std::array<GLenum, 2> glShaderIDs;
    int glShaderIndex = 0;

    for (auto& keyValues : shaderSources)
    {
      GLenum type = keyValues.first;
      const std::string& source = keyValues.second;

      GLuint shader = glCreateShader(type);

      const GLchar* sourceCSTR = source.c_str();
      glShaderSource(shader, 1, &sourceCSTR, 0);

      glCompileShader(shader);

      GLint isCompiled = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE)
      {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> infoLog(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, &infoLog[0]);

        glDeleteShader(shader);

        switch (type)
        {
        case GL_VERTEX_SHADER:
          BS_ERROR("Vertex Shader compilation failure");
          break;
        case GL_GEOMETRY_SHADER:
          BS_ERROR("Geometry Shader compilation failure");
          break;
        case GL_FRAGMENT_SHADER:
          BS_ERROR("Fragment Shader compilation failure");
          break;
        }
        BS_ERROR("{0}", infoLog.data());
        return;
      }
      glAttachShader(program, shader);
      glShaderIDs[glShaderIndex++] = shader;
    }

    m_RendererID = program;
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

      for (auto id : glShaderIDs)
      {
        glDeleteShader(id);
      }

      BS_ERROR("Shader linking failure");
      BS_ERROR("{0}", infoLog.data());
      return;
    }

    for (auto id : glShaderIDs)
    {
      glDetachShader(m_RendererID, id);
      glDeleteShader(id);
    }
  }

}