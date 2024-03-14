#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

static const char* m_MapTiles =
{
  "WWWWWWWWWWWWWWWWWWWWWWWW"
  "WWWWWDDDDDDDDWWWWWWWWWWW"
  "WWWDDDDDDDDDDDDDDWWWWWWW"
  "WWDDDDDDDDDDDDDDDDWWWWWW"
  "WWWDDDDDDDDDDDDDDDDWWWWW"
  "WWWWDDDDDDDDDDDDDDDWWWWW"
  "WWWDDDDDDDDDDDDDDDDDWWWW"
  "WWWDDDDDDDDDWWDDDDDDWWWW"
  "WWWDDDDDDDDDWWWDDDDDWWWW"
  "WWWWDDDDDDDDWWWWDDDDWWWW"
  "WWWDDDDDDDDWWWWWWDDWWWWW"
  "WWWWDDDDDDWWWWWWWWWWWWWW"
  "WWWWWWWWWWWWWWWWWWWWWWWW"
  "WWWWWWWWWWWWWWWWWWWWWWWW"
};

void Sandbox2D::OnAttach()
{
  BS_PROFILE_FUNCTION();

  // VARIABLES
  m_SquareColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  // CAMERA CONTROLLER
  m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

  // TEXTURES
  m_Texture = Buckshot::Texture2D::Create("assets/textures/Checkerboard.png");
  m_SpriteSheet = Buckshot::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
  m_BarrelTexture = Buckshot::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2(8.0f, 1.0f), glm::vec2(128.0f));

  m_TextureMap['W'] = Buckshot::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2(11.0f, 11.0f), glm::vec2(128.0f));
  m_TextureMap['D'] = Buckshot::SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2(6.0f, 11.0f), glm::vec2(128.0f));
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
//     Buckshot::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, 10.0f);
//     for (float y = -5.0f; y < 5.0f; y += 0.5f)
//     {
//       for (float x = -5.0f; x < 5.0f; x += 0.5f)
//       {
//         glm::vec4 color((x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f);
//         Buckshot::Renderer2D::DrawQuad({ x, y, 0.5f }, { 0.5f, 0.5f }, color);
//       }
//     }
    for (uint32_t y = 0; y < m_MapHeight; y++)
    {
      for (uint32_t x = 0; x < m_MapWidth; x++)
      {
        char tile_type = m_MapTiles[x + y * m_MapWidth];
        Buckshot::Ref<Buckshot::SubTexture2D> texture;
        if (m_TextureMap.find(tile_type) != m_TextureMap.end())
          texture = m_TextureMap[tile_type];
        else
          texture = m_BarrelTexture;

        Buckshot::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
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
