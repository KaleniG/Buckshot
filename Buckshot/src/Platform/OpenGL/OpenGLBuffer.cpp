#include <bspch.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Buckshot
{
  // VERTEX BUFFER

  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
  {
    BS_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    BS_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
  }

  void OpenGLVertexBuffer::Bind() const
  {
    BS_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  }

  void OpenGLVertexBuffer::Unbind() const
  {
    BS_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  // INDEX BUFFER

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indicies, uint32_t count)
  {
    BS_PROFILE_FUNCTION();

    m_Count = count;
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    BS_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
  }

  void OpenGLIndexBuffer::Bind() const
  {
    BS_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }

  void OpenGLIndexBuffer::Unbind() const
  {
    BS_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}