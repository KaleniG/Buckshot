#pragma once

#include "Buckshot/Renderer/RendererAPI.h"

namespace Buckshot {

  class RenderCommand
  {
  public:
    inline static void Init()
    {
      BS_PROFILE_FUNCTION();

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

    inline static void SetLineWidth(float width)
    {
      m_RendererAPI->SetLineWidth(width);
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t index_count = 0)
    {
      m_RendererAPI->DrawIndexed(vertexArray, index_count);
    }

    inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertex_count = 0)
    {
      m_RendererAPI->DrawLines(vertexArray, vertex_count);
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
      m_RendererAPI->SetViewport(x, y, width, height);
    }

  private:
    static Scope<RendererAPI> m_RendererAPI;
  };

}