#include <bspch.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLUtils.h"

namespace Buckshot {

  OpenGLVertexArray::OpenGLVertexArray()
  {
    BS_PROFILE_FUNCTION();

    glCreateVertexArrays(1, &m_RendererID);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    BS_PROFILE_FUNCTION();

    glDeleteVertexArrays(1, &m_RendererID);
  }

  void OpenGLVertexArray::Bind() const
  {
    BS_PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
  }

  void OpenGLVertexArray::Unbind() const
  {
    BS_PROFILE_FUNCTION();

    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {
    BS_PROFILE_FUNCTION();

    BS_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout");
    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();


    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
      switch (element.DataType)
      {
      case ShaderDataType::Float:
      case ShaderDataType::Float2:
      case ShaderDataType::Float3:
      case ShaderDataType::Float4:
      case ShaderDataType::Int:
      case ShaderDataType::Int2:
      case ShaderDataType::Int3:
      case ShaderDataType::Int4:
      case ShaderDataType::Bool:
      {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex,
          element.GetComponentCount(),
          ShaderDataTypeToOpenGLBaseType(element.DataType),
          element.Normalized ? GL_TRUE : GL_FALSE,
          layout.GetStride(),
          (const void*)element.Offset);
        m_VertexBufferIndex++;
        break;
      }
      case ShaderDataType::Mat3:
      case ShaderDataType::Mat4:
      {
        uint8_t count = element.GetComponentCount();
        for (uint8_t i = 0; i < count; i++)
        {
          glEnableVertexAttribArray(m_VertexBufferIndex);
          glVertexAttribPointer(m_VertexBufferIndex,
            count,
            ShaderDataTypeToOpenGLBaseType(element.DataType),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            (const void*)(element.Offset + sizeof(float) * count * i));
          glVertexAttribDivisor(m_VertexBufferIndex, 1);
          m_VertexBufferIndex++;
        }
        break;
      }
      default:
        BS_ASSERT(false, "Unknown ShaderDataType while generating the VertexArray");
      }
    }

    m_VertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    BS_PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
  }

}