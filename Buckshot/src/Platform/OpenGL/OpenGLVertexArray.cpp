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
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.DataType), (element.Normalized) ? GL_FALSE : GL_TRUE, layout.GetStride(), (const void*)(intptr_t)element.Offset);
      index++;
    }

    quadVBs.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    BS_PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    quadIB = indexBuffer;
  }

}