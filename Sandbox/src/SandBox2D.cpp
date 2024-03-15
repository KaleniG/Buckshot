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

  static bool docking_enabled = true;

  if (docking_enabled)
  {
    static bool dockspace_open = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Exit")) Buckshot::Application::Get().Close();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::Begin("2D Renderer", (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    auto stats = Buckshot::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %i", stats.DrawCalls);
    ImGui::Text("Quad Count: %i", stats.QuadCount);
    ImGui::Text("Total Vertices: %i", stats.GetTotalVertexCount());
    ImGui::Text("Total Indices: %i", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_NoInputs);
    ImGui::End();

    ImGui::End();
  }
  else
  {
    ImGui::Begin("2D Renderer", (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    auto stats = Buckshot::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %i", stats.DrawCalls);
    ImGui::Text("Quad Count: %i", stats.QuadCount);
    ImGui::Text("Total Vertices: %i", stats.GetTotalVertexCount());
    ImGui::Text("Total Indices: %i", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_NoInputs);
    ImGui::End();
  }

}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}