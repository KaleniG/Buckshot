#pragma once

#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    virtual void Init() override;
    virtual void ClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
  };

}