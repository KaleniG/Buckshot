#pragma once

#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Events/ApplicationEvent.h"
#include "Buckshot/Events/MouseEvent.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  class OrthographicCameraController
  {
  public:
    OrthographicCameraController() = default;
    OrthographicCameraController(float aspect_ratio);

    inline OrthographicCamera& GetCamera() { return m_Camera; }
    inline const OrthographicCamera& GetCamera() const { return m_Camera; }

    inline float GetZoomLevel() const { return m_ZoomLevel; }
    inline void SetZoomLevel(float zoom_level) { m_ZoomLevel = zoom_level; }

    void OnUpdate(Timestep timestep);
    void OnEvent(Event& event);

  private:
    bool OnMouseScrolled(MouseScrolledEvent& event);
    bool OnWindowResized(WindowResizeEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

  private:
    float m_AspectRatio;
    float m_ZoomLevel;
    float m_CameraTranslationSpeed;
    glm::vec3 m_CameraPosition;

    OrthographicCamera m_Camera;
  };

}