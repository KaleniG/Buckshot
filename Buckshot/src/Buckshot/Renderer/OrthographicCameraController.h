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

    void OnResize(float width, float height);

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    const OrthographicCameraBounds& GetCameraBounds() const { return m_Bounds; }
    float GetZoomLevel() const { return m_ZoomLevel; }
    void SetZoomLevel(float zoom_level) { m_ZoomLevel = zoom_level; CalculateView(); }

    void OnUpdate(Timestep timestep);
    void OnEvent(Event& event);

  private:
    void CalculateView();
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