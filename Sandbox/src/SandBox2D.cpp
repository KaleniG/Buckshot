#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

void Sandbox2D::OnAttach()
{
  BS_PROFILE_FUNCTION();

  // VARIABLES
  m_SquareColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  // CAMERA CONTROLLER
  m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

  // TEXTURES
  m_Texture = Buckshot::Texture2D::Create("assets/textures/Checkerboard.png");
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
    static float rotation = 0.0f;
    rotation += timestep * 20.0f;

    Buckshot::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Buckshot::Renderer2D::DrawQuad({ 0.0f,  0.0f,  0.5f }, { 1.0f, 1.0f }, m_SquareColor);
    Buckshot::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f);
    Buckshot::Renderer2D::DrawRotatedQuad({ 0.0f,  0.0f,  0.5f }, { 1.0f, 1.0f }, rotation, m_SquareColor);
    Buckshot::Renderer2D::DrawRotatedQuad({ 0.0f,  0.0f,  0.0f }, { 5.0f, 5.0f }, rotation, m_Texture, 10.0f);
    Buckshot::Renderer2D::EndScene();

    Buckshot::Renderer2D::BeginScene(m_CameraController.GetCamera());
    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
      for (float x = -5.0f; x < 5.0f; x += 0.5f)
      {
        Buckshot::Renderer2D::DrawQuad({ x, y, 0.5f }, { 0.45f, 0.45f }, m_SquareColor);
      }
    }
    Buckshot::Renderer2D::EndScene();
  }

}

void Sandbox2D::OnImGuiRender()
{
  BS_PROFILE_FUNCTION();

  ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f));
  ImGui::SetNextWindowBgAlpha(0.5f);
  ImGuiWindowFlags window_flags =
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoNav;
  ImGui::Begin("Overlay", (bool*)true, window_flags);
  ImGui::Text("Camera Position: x:%1.2f y:%1.2f\n", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y);
  ImGui::Text("Camera ZoomLevel: %1.2f\n", m_CameraController.GetZoomLevel());
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_NoInputs);

  ImGui::Separator();

  auto stats = Buckshot::Renderer2D::GetStats();
  ImGui::Text("Renderer Stats:");
  ImGui::Text(" Draw Calls: %i", stats.DrawCalls);
  ImGui::Text(" Quad Count: %i", stats.QuadCount);
  ImGui::Text(" Total Vertecies: %i", stats.GetTotalVertexCount());
  ImGui::Text(" Total Indecies: %i", stats.GetTotalIndexCount());

  ImGui::End();
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}
