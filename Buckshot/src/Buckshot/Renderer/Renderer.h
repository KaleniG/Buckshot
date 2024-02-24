#pragma once

#include "Buckshot/Renderer/Shader.h"
#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  class Renderer
  {
  public:
    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();
    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
    
    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

  private:
    struct SceneData
    {
      glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;
  };

}