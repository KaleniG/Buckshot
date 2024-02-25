#pragma once

#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  class RenderCommand
  {
  public:
    inline static void Init()
    {
      m_RendererAPI->Init();
    }

    inline static void ClearColor(const glm::vec4& color)
    {
      m_RendererAPI->ClearColor(color);
    }

    inline static void Clear()
    {
      m_RendererAPI->Clear();
    }


    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
      m_RendererAPI->DrawIndexed(vertexArray);
    }

  private:
    static RendererAPI* m_RendererAPI;
  };

}