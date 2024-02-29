#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

template<typename Fn>
class Timer
{
public:
  Timer(const char* name, Fn&& func)
    : m_Name(name), m_Func(func), m_Stopped(false)
  {
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    if (!m_Stopped)
      Stop();
  }

  void Stop()
  {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    m_Stopped = true;

    float duration = (end - start) * 0.001f;

    m_Func({m_Name, duration});
  }

private:
  Fn m_Func;
  bool m_Stopped;
  const char* m_Name;
  std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

void Sandbox2D::OnAttach()
{
  // VARIABLES
  m_SquareColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  // CAMERA CONTROLLER
  m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

  // TEXTURES
  m_Texture = Buckshot::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Buckshot::Timestep timestep)
{
  PROFILE_SCOPE("Sandbox2D::OnUpdate");
  m_CameraController.OnUpdate(timestep);


  {
    PROFILE_SCOPE("Sandbox2::Startup");
    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();
  }

  {
    PROFILE_SCOPE("Sandbox2::BeginScene");
    Buckshot::Renderer2D::BeginScene(m_CameraController.GetCamera());
  }


  {
    PROFILE_SCOPE("Sandbox2::Drawing");
    Buckshot::Renderer2D::DrawQuad({ 1.0f,  0.0f,  0.0f }, { 0.7f, 0.7f }, m_SquareColor);
    Buckshot::Renderer2D::DrawQuad({ -1.0f,  0.0f,  0.0f }, { 0.7f, 0.7f }, m_SquareColor);
    Buckshot::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.5f }, { 5.0f, 5.0f }, m_Texture);
  }

  Buckshot::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
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

  for (auto result : m_ProfileResults)
  {
    char label[50];
    strcpy(label, "%0.3fms ");
    strcat(label, result.Name);
    ImGui::Text(label, result.Time);
  }
  m_ProfileResults.clear();

  ImGui::End();
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}
