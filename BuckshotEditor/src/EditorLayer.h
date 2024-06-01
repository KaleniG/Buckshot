#pragma once

#include <bspch.h>
#include <Buckshot.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Buckshot {

  class EditorLayer : public Layer
  {
  public:
    EditorLayer();
    ~EditorLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Timestep timestep) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

  private:
    void OnOverlayRender();

    void OnScenePlay();
    void OnSceneStop();
    void OnScenePause();
    void OnSceneSimulate();

    bool OnKeyPressed(KeyPressedEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

    void NewProject();
    void OpenProject(const std::filesystem::path filepath);
    void SaveProject();

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path filepath);
    void SaveSceneAs();
    void SaveScene();

    void OnEntityDuplicate();
    void OnEntityDelete();

    void UI_Toolbar();

  private:
    // GENERAL
    EditorCamera m_EditorCamera;
    Ref<Scene> m_ActiveScene;
    Ref<Scene> m_EditorScene;
    std::string m_CurrentlyOpenScene = std::string();

    enum class SceneState
    {
      Play,
      Edit,
      Simulate
    };

    SceneState m_SceneState = SceneState::Edit;

    // GUI
    Ref<Texture2D> m_IconPlay;
    Ref<Texture2D> m_IconStop;
    Ref<Texture2D> m_IconStep;
    Ref<Texture2D> m_IconPause;
    Ref<Texture2D> m_IconUnpause;
    Ref<Texture2D> m_IconSimulate;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    glm::vec2 m_ViewportBounds[2];
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    SceneHierarchyPanel m_SceneHierarchyPanel;
    Scope<ContentBrowserPanel> m_ContentBrowserPanel;

    // Gizmos
    int m_GizmoType = -1;
    Entity m_HoveredEntity;
    bool m_ShowPhysicsColliders = false;
  };

}