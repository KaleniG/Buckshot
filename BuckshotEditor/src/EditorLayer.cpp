#include <ImGui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Buckshot {

  EditorLayer::EditorLayer()
    : Layer("EditorLayer") {}

  void EditorLayer::OnAttach()
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);

    m_EditorCamera = EditorCamera(30.0f, 1.788f, 0.1f, 1000.0f);

    m_ActiveScene = CreateRef<Scene>();
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
      m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    if (m_ViewportFocused)
      m_EditorCamera.OnUpdate(timestep);

    // Render
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();
    // Update scene
    m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
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
    ImGuiStyle& style = ImGui::GetStyle();

    float min_window_size_x_default = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = min_window_size_x_default;

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New", "Ctrl + N"))
        {
          NewScene();
        }

        if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S"))
        {
          SaveSceneAs();
        }

        if (ImGui::MenuItem("Open...", "Ctrl + O"))
        {
          OpenScene();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Exit")) 
        {
          Application::Get().Close();
        }

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
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
    ImGui::End();

    // VIEWPORT WINDOW
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    // GIZMOS
    Entity selected_entity = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selected_entity && m_GizmoType != -1)
    {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();
      float window_width = (float)ImGui::GetWindowWidth();
      float window_height = (float)ImGui::GetWindowHeight();
      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

      // GIZMO::CAMERA::RUNTIME
      // auto camera_entity = m_ActiveScene->GetPrimaryCameraEntity();
      // const auto& camera = camera_entity.GetComponent<CameraComponent>().Camera;
      // const glm::mat4& camera_projection = camera.GetProjection();
      // glm::mat4 camera_view = glm::inverse(camera_entity.GetComponent<TransformComponent>().GetTransform());

      // GIZMO::CAMERA::EDITOR
      const glm::mat4& camera_projection = m_EditorCamera.GetProjection();
      glm::mat4 camera_view = m_EditorCamera.GetViewMatrix();


      // GIZMO::SUBJECT
      auto& selected_entity_transform_component = selected_entity.GetComponent<TransformComponent>();
      glm::mat4 selected_entity_transform = selected_entity_transform_component.GetTransform();

      // GIZMO::SNAPPING
      bool snap = Input::IsKeyPressed(Key::RightControl) || Input::IsKeyPressed(Key::LeftControl);
      float snap_value = 0.5f; // Snap for 0.5 distance units for translation/scaling;
      if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        snap_value = 45.0f; // Snap for 45.0 degrees for rotation;
      float snap_values[3] = { snap_value, snap_value, snap_value };

      // GIZMO::TRANSFORMATIONS
      ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(selected_entity_transform), nullptr, snap ? snap_values : nullptr);

      if (ImGuizmo::IsUsing())
      {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Math::DecomposeTransform(selected_entity_transform, position, rotation, scale);

        glm::vec3 delta_rotation = rotation - selected_entity_transform_component.Rotation;
        selected_entity_transform_component.Position = position;
        selected_entity_transform_component.Rotation += delta_rotation;
        selected_entity_transform_component.Scale = scale;
      }
    }


    ImGui::End();
    ImGui::PopStyleVar();

    // END
    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& event)
  {
    m_EditorCamera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(BS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
  }

  bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
  {
    if (event.GetRepeatCount() > 0)
      return false;

    bool control_pressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
    bool shift_pressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

    switch (event.GetKeyCode())
    {
    // Shortcuts
    case Key::N:
    {
      if (control_pressed)
        NewScene();
      return false;
    }
    case Key::O:
    {
      if (control_pressed)
        OpenScene();
      return false;
    }
    case Key::S:
    {
      if (control_pressed && shift_pressed)
        SaveSceneAs();
      return false;
    }

    // Gizmo

    case Key::Z:
    {
      m_GizmoType = -1;
      return false;
    }
    case Key::X:
    {
      m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
      return false;
    }
    case Key::C:
    {
      m_GizmoType = ImGuizmo::OPERATION::ROTATE;
      return false;
    }
    case Key::V:
    {
      m_GizmoType = ImGuizmo::OPERATION::SCALE;
      return false;
    }

    }
    return false;
  }

  void EditorLayer::NewScene()
  {
    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OpenScene()
  {
    std::string filepath = FileDialogs::OpenFile("Buckshot Scene (*.bshot)\0*.bshot\0");

    if (!filepath.empty())
    {
      m_ActiveScene = CreateRef<Scene>();
      m_SceneHierarchyPanel.SetContext(m_ActiveScene);

      SceneSerializer serializer(m_ActiveScene);
      serializer.Deserialize(filepath);
      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }
  }

  void EditorLayer::SaveSceneAs()
  {
    std::string filepath = FileDialogs::SaveFile("Buckshot Scene (*.bshot)\0*.bshot\0");

    if (!filepath.empty())
    {
      SceneSerializer serializer(m_ActiveScene);
      serializer.Serialize(filepath);
    }
  }

}