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
    m_IconPlay = Texture2D::Create("assets/textures/IconPlay.png");
    m_IconStop = Texture2D::Create("assets/textures/IconStop.png");
    m_IconStep = Texture2D::Create("assets/textures/IconStep.png");
    m_IconPause = Texture2D::Create("assets/textures/IconPause.png");
    m_IconUnpause = Texture2D::Create("assets/textures/IconPlay.png");
    m_IconSimulate = Texture2D::Create("assets/textures/IconSimulate.png");

    FramebufferSpecification fbSpec;
    fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::DEPTH24STENCIL8 };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);

    m_EditorCamera = EditorCamera(30.0f, 1.788f, 0.1f, 1000.0f);

    m_ActiveScene = CreateRef<Scene>();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
    if (commandLineArgs.Count > 1)
    {
      auto project_filepath = commandLineArgs[1];
      OpenProject(project_filepath);
    }
    else
    {
      if (!OpenProject())
      {
        // TEMPORARY
        BS_FATAL("Can't start the editor without an initial project");
        Application::Get().Close();

      }
    }

    Renderer2D::SetLineWidth(4.0f);
  }

  void EditorLayer::OnDetach()
  {
  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
    // Editor viewport resizing
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    }

    // Render
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();
    m_Framebuffer->ClearAttachment(1, -1);

    // Update scene
    switch (m_SceneState)
    {
    case SceneState::Edit:
    {
      if (m_ViewportFocused)
        m_EditorCamera.OnUpdate(timestep);

      m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
      break;
    }
    case SceneState::Simulate:
    {
      if (m_ViewportFocused)
        m_EditorCamera.OnUpdate(timestep);

      m_ActiveScene->OnUpdateSimulation(timestep, m_EditorCamera);
      break;
    }
    case SceneState::Play:
    {
      m_GizmoType = -1;
      m_ActiveScene->OnUpdateRuntime(timestep);
      break;
    }
    }

    OnOverlayRender();

    // Gizmos related stuff
    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_ViewportBounds[0].x;
    my -= m_ViewportBounds[0].y;
    glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
    my = viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
    {
      int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
      if (pixelData == -1)
        m_HoveredEntity = Entity();
      else
        m_HoveredEntity = Entity((entt::entity)pixelData, m_ActiveScene.get());
    }

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
        if (ImGui::MenuItem("Open Project"))
        {
          OpenProject();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("New Scene", "Ctrl + N"))
        {
          NewScene();
        }

        if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
        {
          SaveScene();
        }

        if (ImGui::MenuItem("Save Scene As...", "Ctrl + Shift + S"))
        {
          SaveSceneAs();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
        {
          Application::Get().Close();
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Scene"))
      {
        if (ImGui::MenuItem("New Entity"))
        {
          m_ActiveScene->CreateEntity("NewEntity");
        }

        if (ImGui::MenuItem("Duplicate Entity", "Ctrl + D", nullptr, (bool)m_SceneHierarchyPanel.GetSelectedEntity()))
        {
          Entity selected_entity = m_SceneHierarchyPanel.GetSelectedEntity();
          m_ActiveScene->DuplicateEntity(selected_entity);
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Script"))
      {
        if (ImGui::MenuItem("Reaload", "Ctrl + R"))
        {
          if (m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate)
            OnSceneStop();
          ScriptEngine::ReloadAssembly();
        }

        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // SCENE HIEARACHY PANEL
    m_SceneHierarchyPanel.OnImGuiRender();

    // CONTENT BRWOSER PANEL
    m_ContentBrowserPanel->OnImGuiRender();

    // STATS PANEL
    ImGui::Begin("Stats");
    auto stats = Renderer2D::GetStats();
    std::string hovered_entity_name = "None";
    if (m_HoveredEntity) hovered_entity_name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertices Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices Count: %d", stats.GetTotalIndexCount());
    ImGui::Text("Hovered Entity: %s", hovered_entity_name.c_str());
    ImGui::End();

    // STATS PANEL
    ImGui::Begin("Settings");
    ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
    ImGui::End();

    // VIEWPORT PANEL
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    // MOUSE-PICKING
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // SETUP A PROPER EVENT_BLOCKING SYSTEM!!!!
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_FILE"))
      {
        const wchar_t* path = (const wchar_t*)payload->Data;
        OpenScene(path);
      }
      ImGui::EndDragDropTarget();
    }


    // GIZMOS
    Entity selected_entity = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selected_entity && m_GizmoType != -1)
    {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();

      ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

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

    UI_Toolbar();

    // END
    ImGui::End();
  }

  void EditorLayer::UI_Toolbar()
  {
    auto& colors = ImGui::GetStyle().Colors;
    const auto& button_hovered = colors[ImGuiCol_ButtonHovered];
    const auto& button_active = colors[ImGuiCol_ButtonActive];

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(button_active.x, button_active.y, button_active.z, 0.5f));
    ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 4.0f;

    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x / 2.0f) - (size / 2.0f) * (m_SceneState != SceneState::Edit ? 3.0f : 2.0f));
    {
      Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
      if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
      {
        if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
        {
          OnScenePlay();
        }
        else if (m_SceneState == SceneState::Play)
        {
          OnSceneStop();
        }
      }
    }
    ImGui::SameLine();
    {
      Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
      if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
      {
        if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
        {
          OnSceneSimulate();
        }
        else if (m_SceneState == SceneState::Simulate)
        {
          OnSceneStop();
        }
      }
    }
    if (m_SceneState != SceneState::Edit)
    {
      bool is_paused = m_ActiveScene->IsPaused();

      {
        ImGui::SameLine();
        Ref<Texture2D> icon = is_paused ? m_IconUnpause : m_IconPause;

        if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
        {
          m_ActiveScene->SetPaused(!is_paused);
        }
      }

      if (is_paused)
      {
        ImGui::SameLine();
        Ref<Texture2D> icon = m_IconStep;

        if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
        {
          m_ActiveScene->Step(5);
        }
      }
    }

    ImGui::End();
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
  }

  void EditorLayer::OnEvent(Event& event)
  {
    m_EditorCamera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(BS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(BS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
  }

  bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
  {
    if (event.GetRepeatCount() > 0 || m_SceneState == SceneState::Play)
      return false;

    bool control_pressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
    bool shift_pressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

    switch (event.GetKeyCode())
    {
      // File Options Shortcuts
    case Key::N:
    {
      if (control_pressed)
        NewScene();
      return false;
    }
    case Key::S:
    {
      if (control_pressed)
      {
        if (shift_pressed)
          SaveSceneAs();
        else
          SaveScene();
      }
      return false;
    }

    // Entities Shortcuts

    case Key::Delete:
    {
      OnEntityDelete();
    }
    case Key::D:
    {
      if (control_pressed)
      {
        OnEntityDuplicate();
      }
    }

    // Scripting Shortcuts

    case Key::R:
    {
      if (control_pressed)
      {
        if (m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate)
          OnSceneStop();
        ScriptEngine::ReloadAssembly();
      }
      return false;
    }

    // Gizmo

    case Key::Z:
    {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = -1;
      return false;
    }
    case Key::X:
    {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
      return false;
    }
    case Key::C:
    {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::ROTATE;
      return false;
    }
    case Key::V:
    {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::SCALE;
      return false;
    }
    }
    return false;
  }

  bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
  {
    if (event.GetMouseButton() == Mouse::ButtonLeft)
    {
      if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
        m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
    }
    return false;
  }

  void EditorLayer::NewProject()
  {
    Project::New();
  }

  bool EditorLayer::OpenProject()
  {
    std::string filepath = FileDialogs::OpenFile("Buckshot Project (*.bproj)\0*.bproj\0");
    if (filepath.empty())
      return false;

    OpenProject(filepath);
    return true;
  }

  void EditorLayer::OpenProject(const std::filesystem::path filepath)
  {
    if (Project::Load(filepath))
    {
      auto config = Project::GetActive()->GetConfiguration();
      auto start_scene_path = Project::GetFileSystemAssetPath(config.StartScenePath);
      OpenScene(start_scene_path);
      m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
    }
  }

  void EditorLayer::SaveProject()
  {
    // LATER
  }

  void EditorLayer::NewScene()
  {
    m_EditorScene = CreateRef<Scene>();
    m_ActiveScene = m_EditorScene;
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OpenScene()
  {
    std::string filepath = FileDialogs::OpenFile("Buckshot Scene (*.bshot)\0*.bshot\0");
    if (!filepath.empty())
      OpenScene(filepath);
  }

  void EditorLayer::OpenScene(const std::filesystem::path filepath)
  {
    if (m_SceneState != SceneState::Edit)
      OnSceneStop();

    if (filepath.extension() != ".bshot")
    {
      BS_WARN("Could not load {0}, not a scene file", filepath.string());
      return;
    }

    Ref<Scene> new_scene = CreateRef<Scene>();
    SceneSerializer serializer(new_scene);
    if (serializer.Deserialize(filepath.string()))
    {
      m_EditorScene = new_scene;
      m_CurrentlyOpenScene = filepath.string();
      m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_SceneHierarchyPanel.SetContext(m_EditorScene);
      m_ActiveScene = m_EditorScene;
    }

  }

  void EditorLayer::SaveSceneAs()
  {
    std::string filepath = FileDialogs::SaveFile("Buckshot Scene (*.bshot)\0*.bshot\0");

    if (!filepath.empty())
    {
      SceneSerializer serializer(m_ActiveScene);
      serializer.Serialize(filepath);
      m_CurrentlyOpenScene = filepath;
    }
  }

  void EditorLayer::SaveScene()
  {
    if (m_CurrentlyOpenScene.empty())
    {
      SaveSceneAs();
    }
    else
    {
      SceneSerializer serializer(m_ActiveScene);
      serializer.Serialize(m_CurrentlyOpenScene);
    }
  }

  void EditorLayer::OnEntityDuplicate()
  {
    if (m_SceneHierarchyPanel.GetSelectedEntity())
    {
      Entity selected_entity = m_SceneHierarchyPanel.GetSelectedEntity();
      m_SceneHierarchyPanel.SetSelectedEntity(m_ActiveScene->DuplicateEntity(selected_entity));
    }
  }

  void EditorLayer::OnEntityDelete()
  {
    if (m_SceneHierarchyPanel.GetSelectedEntity())
    {
      m_ActiveScene->DestroyEntity(m_SceneHierarchyPanel.GetSelectedEntity());
      m_SceneHierarchyPanel.SetSelectedEntity({});
    }
  }

  void EditorLayer::OnOverlayRender()
  {
    if (m_SceneState == SceneState::Play)
    {
      Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
      if (!camera)
        return;
      Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
    }
    else
    {
      Renderer2D::BeginScene(m_EditorCamera);
    }

    if (m_ShowPhysicsColliders)
    {
      // RECT
      {
        auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
        for (auto entity : view)
        {
          auto transform = view.get<TransformComponent>(entity);
          auto bc2d = view.get<BoxCollider2DComponent>(entity);

          glm::vec3 translation = transform.Position + glm::vec3(bc2d.Offset, 0.001f);
          glm::vec3 scale = transform.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
          glm::mat4 offset = glm::translate(glm::mat4(1.0f), translation)
            * glm::rotate(glm::mat4(1.0f), transform.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), scale);

          Renderer2D::DrawRect(offset, glm::vec4(0, 1, 0, 1));
        }
      }

      // CIRCLE | ISSUE: Cannot make elipse circle coliders, ex. when resizing the circle the overlay resizes but the collider doesn't have the same shape)
      {
        auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
        for (auto entity : view)
        {
          auto transform = view.get<TransformComponent>(entity);
          auto cc2d = view.get<CircleCollider2DComponent>(entity);

          glm::vec3 translation = transform.Position + glm::vec3(cc2d.Offset, 0.001f);
          glm::vec3 scale = transform.Scale * glm::vec3(cc2d.Radius * 2.0f);
          glm::mat4 offset = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);

          Renderer2D::DrawCircle(offset, glm::vec4(0, 1, 0, 1), 0.03f);
        }
      }
    }

    if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
    {
      const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
      Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
    }


    Renderer2D::EndScene();
  }

  void EditorLayer::OnScenePlay()
  {
    if (m_SceneState == SceneState::Simulate)
      OnSceneStop();

    m_SceneState = SceneState::Play;
    m_EditorScene = Scene::Copy(m_ActiveScene);
    m_ActiveScene = Scene::Copy(m_EditorScene);
    m_ActiveScene->OnRuntimeStart();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OnSceneSimulate()
  {
    if (m_SceneState == SceneState::Play)
      OnSceneStop();

    m_SceneState = SceneState::Simulate;
    m_EditorScene = Scene::Copy(m_ActiveScene);
    m_ActiveScene = Scene::Copy(m_EditorScene);
    m_ActiveScene->OnSimulationStart();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
  }

  void EditorLayer::OnSceneStop()
  {
    CL_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate, "Invalid Scene State at this stage");

    if (m_SceneState == SceneState::Play)
      m_ActiveScene->OnRuntimeStop();
    else if (m_SceneState == SceneState::Simulate)
      m_ActiveScene->OnSimulationStop();

    m_SceneState = SceneState::Edit;
    m_ActiveScene = m_EditorScene;
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OnScenePause()
  {
    m_ActiveScene->SetPaused(true);
  }

}