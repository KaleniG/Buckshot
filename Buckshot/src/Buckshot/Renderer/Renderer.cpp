#include <bspch.h>

#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Renderer/Renderer2D.h"

namespace Buckshot {

  void Renderer::OnWindowResize(uint32_t width, uint32_t height)
  {
    RenderCommand::SetViewport(0, 0, width, height);
  }

  void Renderer::Init()
  {
    BS_PROFILE_FUNCTION();

    RenderCommand::Init();
    Renderer2D::Init();
  }

  void Renderer::Shutdown()
  {
    BS_PROFILE_FUNCTION();

    Renderer2D::Shutdown();
  }

  void Renderer::BeginScene(OrthographicCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene()
  {
    BS_PROFILE_FUNCTION();
  }

  void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
  {
    BS_PROFILE_FUNCTION();

    shader->Bind();
    shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
  }

  Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

}