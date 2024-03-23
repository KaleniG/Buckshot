#pragma once

#include <Buckshot.h>

#include "Panels/SceneHierarchyPanel.h"

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
    void NewScene();
    void OpenScene();
    void SaveSceneAs();

  private:
    Ref<Scene> m_ActiveScene;

    // GUI
    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    SceneHierarchyPanel m_SceneHierarchyPanel;

    // Gizmo
    int m_GizmoType = -1;
  };

}