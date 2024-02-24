#pragma once

#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  class RenderCommand
  {
  public:
    inline static void ClearColor(const glm::vec4& color)
    {
      m_RendererAPI->ClearColor(color);
    }

    inline static void Clear()
    {
      m_RendererAPI->Clear();
    }


    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {
      m_RendererAPI->DrawIndexed(vertexArray);
    }

  private:
    static RendererAPI* m_RendererAPI;
  };

}