#pragma once

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
    bool OnKeyPressed(KeyPressedEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
    void NewScene();
    void OpenScene();
    void SaveSceneAs();

  private:
    EditorCamera m_EditorCamera;
    Ref<Scene> m_ActiveScene;

    // GUI
    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    glm::vec2 m_ViewportBounds[2];
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPanel m_ContentBrowserPanel;

    // Gizmo
    int m_GizmoType = -1;
    Entity m_HoveredEntity;
  };

}