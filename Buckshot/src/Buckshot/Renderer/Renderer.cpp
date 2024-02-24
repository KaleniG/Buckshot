#include <bspch.h>

#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {

  void Renderer::BeginScene(OrthographicCamera& camera)
  {
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene()
  {

  }

  void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
  {
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
    shader->UploadUniformMat4("u_Transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
  }

  Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

}