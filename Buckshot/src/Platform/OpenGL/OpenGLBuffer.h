#pragma once

#include "Buckshot/Renderer/Buffers.h"

namespace Buckshot {

  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const BufferLayout& GetLayout() override { return m_Layout; }
    virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    virtual void SetData(const void* data, uint32_t size) override;

  private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
  };

  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
    virtual ~OpenGLIndexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual uint32_t GetCount() const override { return m_Count; }

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;
  };

}