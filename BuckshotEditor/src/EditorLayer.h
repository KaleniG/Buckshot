#pragma once

#include <Buckshot.h>

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
    OrthographicCameraController m_CameraController;
    Entity m_SquareEntity;
    Entity m_CameraEntity;
    Entity m_SecondCamera;

    Ref<Scene> m_ActiveScene;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;
    Ref<SubTexture2D> m_BarrelTexture;

    bool m_PrimaryCamera = true;

    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
  };

}