#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Buckshot {

  EditorLayer::EditorLayer()
  {
    Layer("EditorLayer");
  }

  void EditorLayer::OnAttach()
  {
    BS_PROFILE_FUNCTION();

    // CAMERA CONTROLLER
    m_CameraController = OrthographicCameraController(1280.0f / 720.0f);

    // TEXTURES
    m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
    m_SpriteSheet = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
    m_BarrelTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, glm::vec2(8.0f, 2.0f), glm::vec2(128.0f));

    // FRAMEBUFFER
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
  }

  void EditorLayer::OnDetach()
  {
    BS_PROFILE_FUNCTION();
  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
    BS_PROFILE_FUNCTION();

    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    if (m_ViewportFocused || m_ViewportHovered)
      m_CameraController.OnUpdate(timestep);

    Renderer2D::ResetStats();
    {
      BS_PROFILE_SCOPE("Sandbox2::Startup");
      m_Framebuffer->Bind();
      RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
      RenderCommand::Clear();
    }

    {
      BS_PROFILE_SCOPE("Sandbox2::Drawing");
      Renderer2D::BeginScene(m_CameraController.GetCamera());
      Renderer2D::DrawQuad({ 0,0 }, { 1,1 }, m_BarrelTexture);
      Renderer2D::DrawQuad({ 0,1 }, { 1,1 }, m_BarrelTexture);
      Renderer2D::DrawQuad({ 0,2 }, { 1,1 }, m_BarrelTexture);
      Renderer2D::DrawQuad({ 0,3 }, { 1,1 }, m_BarrelTexture);
      Renderer2D::EndScene();
      m_Framebuffer->Unbind();
    }

  }

  void EditorLayer::OnImGuiRender()
  {
    BS_PROFILE_FUNCTION();
    
    bool dockspaceOpen = true;
    bool opt_fullscreen_persistant = true;
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
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
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
        if (ImGui::MenuItem("Exit")) Application::Get().Close();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    ImGui::Begin("Settings");
    auto stats = Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads Count: %d", stats.QuadCount);
    ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0, });
    ImGui::Begin("Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewport_panel_size.x, viewport_panel_size.y };
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, {m_ViewportSize.x, m_ViewportSize.y}, {0, 1}, {1, 0});
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& event)
  {
    m_CameraController.OnEvent(event);
  }

}