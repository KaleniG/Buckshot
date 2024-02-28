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

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
      m_RendererAPI->SetViewport(x, y, width, height);
    }

  private:
    static RendererAPI* m_RendererAPI;
  };

}