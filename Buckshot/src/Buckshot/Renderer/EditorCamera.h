#pragma once

#include <glm/glm.hpp>

#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Events/MouseEvent.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/Renderer/Camera.h"

namespace Buckshot {

  class EditorCamera : public Camera
  {
  public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip);

    void OnUpdate(Timestep timestep);
    void OnEvent(Event& event);

    float GetDistance() const { return m_Distance; }
    void SetDistance(float distance) { m_Distance = distance; }

    void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    const glm::vec3& GetPosition() const { return m_Position; }
    glm::quat GetOrientation() const;

    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }

  private:
    void UpdateProjection();
    void UpdateView();

    bool OnMouseScroll(MouseScrolledEvent& e);

    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

  private:
    float m_FOV = 45.0f;
    float m_AspectRatio = 1.778f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;

    glm::mat4 m_ViewMatrix;
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

    float m_Distance = 10.0f;
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;

    float m_ViewportWidth = 1280;
    float m_ViewportHeight = 720;
  };

}