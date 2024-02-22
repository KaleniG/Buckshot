#pragma once

#include <string>

namespace Buckshot
{

  class Shader
  {
  public:
    Shader(const std::string& vertSource, const std::string& fragSource);
    ~Shader();

    void Bind() const;
    void Unbind() const;

  private:
    uint32_t m_RendererID;

  };

}