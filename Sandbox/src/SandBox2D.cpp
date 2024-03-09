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

  {
    BS_PROFILE_SCOPE("Sandbox2::Startup");
    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();
    Buckshot::Renderer2D::BeginScene(m_CameraController.GetCamera());
  }

  {
    BS_PROFILE_SCOPE("Sandbox2::Drawing");
    Buckshot::Renderer2D::DrawQuad({ 1.0f,  0.0f,  0.0f }, { 0.7f, 0.7f }, m_SquareColor);
    Buckshot::Renderer2D::DrawQuad({ -1.0f,  0.0f,  0.0f }, { 0.7f, 0.7f }, m_SquareColor);
    Buckshot::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.5f }, { 5.0f, 5.0f }, m_Texture);
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
  ImGui::End();
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}
