#pragma once

#include <glad/glad.h>
#include "Buckshot/Log.h"
#include "Buckshot/Core.h"
#include "Buckshot/Renderer/Buffers.h"

namespace Buckshot {

  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::None:    break;
    case ShaderDataType::Bool:    return GL_BOOL;
    case ShaderDataType::Float:   return GL_FLOAT;
    case ShaderDataType::Float2:  return GL_FLOAT;
    case ShaderDataType::Float3:  return GL_FLOAT;
    case ShaderDataType::Float4:  return GL_FLOAT;
    case ShaderDataType::Int:     return GL_INT;
    case ShaderDataType::Int2:    return GL_INT;
    case ShaderDataType::Int3:    return GL_INT;
    case ShaderDataType::Int4:    return GL_INT;
    case ShaderDataType::Mat3:    return GL_FLOAT;
    case ShaderDataType::Mat4:    return GL_FLOAT;
    }

    BS_ASSERT(false, "Unknown ShaderDataType specified")
    return 0;
  }

}