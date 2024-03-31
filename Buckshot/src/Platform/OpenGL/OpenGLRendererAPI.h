#pragma once

#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    virtual void Init() override;
    virtual void ClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void SetLineWidth(float width) override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t index_count = 0) override;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertex_count = 0) override;
  };

}