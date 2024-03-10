#pragma once

#include "Buckshot/Renderer/VertexArray.h"

namespace Buckshot {

  class OpenGLVertexArray : public VertexArray
  {
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return quadVBs; }
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return quadIB; }

  public:
    uint32_t m_RendererID;
    std::vector<Ref<VertexBuffer>> quadVBs;
    Ref<IndexBuffer> quadIB;
  };

}