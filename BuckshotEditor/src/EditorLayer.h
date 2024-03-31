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
    void OnScenePlay();
    void OnSceneStop();

    bool OnKeyPressed(KeyPressedEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path filepath);
    void SaveSceneAs();
    void SaveScene();

    void OnEntityDuplicate();
    void OnEntityDelete();

    void UI_Toolbar();

  private:
    EditorCamera m_EditorCamera;
    Ref<Scene> m_ActiveScene;
    Ref<Scene> m_RuntimeScene;
    Ref<Scene> m_EditorScene;
    std::string m_CurrentlyOpenScene = std::string();

    enum class SceneState
    {
      Edit,
      Play
    };

    SceneState m_SceneState = SceneState::Edit;

    // GUI
    Ref<Texture2D> m_IconStop;
    Ref<Texture2D> m_IconPlay;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    glm::vec2 m_ViewportBounds[2];
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPanel m_ContentBrowserPanel;

    // Gizmos
    int m_GizmoType = -1;
    Entity m_HoveredEntity;
  };

}