#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

void Sandbox2D::OnAttach()
{
  BS_PROFILE_FUNCTION();

  // CAMERA CONTROLLER
  m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

  // TEXTURES
  m_Texture = Buckshot::Texture2D::Create("assets/textures/Checkerboard.png");
  m_SpriteSheet = Buckshot::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
  m_BarrelTexture = Buckshot::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2(8.0f, 1.0f), glm::vec2(128.0f));
}

void Sandbox2D::OnDetach()
{
  BS_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Buckshot::Timestep timestep)
{
  BS_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(timestep);

  Buckshot::Renderer2D::ResetStats();
  {
    BS_PROFILE_SCOPE("Sandbox2::Startup");
    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();
  }

  {
    BS_PROFILE_SCOPE("Sandbox2::Drawing");
    Buckshot::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Buckshot::Renderer2D::DrawQuad({ 0,0 }, { 1,1 }, m_BarrelTexture);
    Buckshot::Renderer2D::DrawQuad({ 0,1 }, { 1,1 }, m_BarrelTexture);
    Buckshot::Renderer2D::DrawQuad({ 0,2 }, { 1,1 }, m_BarrelTexture);
    Buckshot::Renderer2D::DrawQuad({ 0,3 }, { 1,1 }, m_BarrelTexture);
    Buckshot::Renderer2D::EndScene();
  }

}

void Sandbox2D::OnImGuiRender()
{
  BS_PROFILE_FUNCTION();

  ImGui::Begin("Settings");
  auto stats = Buckshot::Renderer2D::GetStats();
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.DrawCalls);
  ImGui::Text("Quads: %d", stats.QuadCount);
  ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
  ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

  uint32_t textureID = m_Texture->GetRendererID();
  ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
  ImGui::End();
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}