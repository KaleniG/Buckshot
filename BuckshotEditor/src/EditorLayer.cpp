#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Buckshot {

  EditorLayer::EditorLayer()
    : Layer("EditorLayer") {}

  void EditorLayer::OnAttach()
  {
    m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");

    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    m_ActiveScene = CreateRef<Scene>();
    // Entity
    auto square = m_ActiveScene->CreateEntity("Square");
    square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
    m_SquareEntity = square;

    m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
    m_CameraEntity.AddComponent<CameraComponent>();

    m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
    m_SecondCamera.AddComponent<CameraComponent>().Primary = false;

    class CameraController : public ScriptableEntity
    {
    public:
      void OnCreate()
      {
      }

      void OnDestroy()
      {

      }

      void OnUpdate(Timestep timestep)
      {
        auto& transform = GetComponent<TransformComponent>().Transform;
        float speed = 2.0f;
        if (Input::IsKeyPressed(Key::A))
          transform[3][0] -= speed * timestep;
        if (Input::IsKeyPressed(Key::D))
          transform[3][0] += speed * timestep;
        if (Input::IsKeyPressed(Key::W))
          transform[3][1] += speed * timestep;
        if (Input::IsKeyPressed(Key::S))
          transform[3][1] -= speed * timestep;
      }
    };

    m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OnDetach()
  {
  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    if (m_ViewportFocused)
      m_CameraController.OnUpdate(timestep);
    // Render
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();
    // Update scene
    m_ActiveScene->OnUpdate(timestep);
    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnImGuiRender()
  {
    bool dockspaceOpen = true;
    bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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
    ImGui::Begin("Dockspace", &dockspaceOpen, window_flags);
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

    // SCENE HIEARACHY
    m_SceneHierarchyPanel.OnImGuiRender();

    // STATS WINDOW
    ImGui::Begin("Stats");
    auto stats = Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();

    // VIEWPORT WINDOW
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

    // END
    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& event)
  {
    m_CameraController.OnEvent(event);
  }

}