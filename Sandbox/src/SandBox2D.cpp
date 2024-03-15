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

  // FRAMEBUFFER
  Buckshot::FramebufferSpecification fbSpec;
  fbSpec.Width = 1280;
  fbSpec.Height = 720;
  m_Framebuffer = Buckshot::Framebuffer::Create(fbSpec);
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
    m_Framebuffer->Bind();
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
    m_Framebuffer->Unbind();
  }

}

void Sandbox2D::OnImGuiRender()
{
  BS_PROFILE_FUNCTION();

  static bool dockingEnabled = true;
  if (dockingEnabled)
  {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
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
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();
    if (opt_fullscreen)
      ImGui::PopStyleVar(2);
    // DockSpace
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
        // Disabling fullscreen would allow the window to be moved to the front of other windows, 
        // which we can't undo at the moment without finer window depth/z control.
        //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
        if (ImGui::MenuItem("Exit")) Buckshot::Application::Get().Close();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    ImGui::Begin("Settings");
    auto stats = Buckshot::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

    ImGui::Begin("Renderer");
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
    ImGui::End();

    ImGui::End();
  }
  else
  {
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
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}