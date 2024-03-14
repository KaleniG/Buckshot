#pragma once

#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Events/ApplicationEvent.h"
#include "Buckshot/Events/MouseEvent.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  struct OrthographicCameraBounds
  {
    float Left;
    float Right;
    float Bottom;
    float Top;

    float GetWidth() { return Right - Left; }
    float GetHeight() { return Top - Bottom; }
  };

  class OrthographicCameraController
  {
  public:
    OrthographicCameraController() = default;
    OrthographicCameraController(float aspect_ratio);

    inline OrthographicCamera& GetCamera() { return m_Camera; }
    inline const OrthographicCamera& GetCamera() const { return m_Camera; }

    inline const OrthographicCameraBounds& GetCameraBounds() const { return m_Bounds; }
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

    OrthographicCameraBounds m_Bounds;
    OrthographicCamera m_Camera;
  };

}