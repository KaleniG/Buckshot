#pragma once

#include "Buckshot/Renderer/Shader.h"
#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  class Renderer
  {
  public:
    static void Init();
    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();
    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    
    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  private:
    struct SceneData
    {
      glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
  };

}