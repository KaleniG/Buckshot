#pragma once

#include "Buckshot/Renderer/Camera.h"

namespace Buckshot {

  class SceneCamera : public Camera
  {
  public:
    enum class ProjectionType
    {
      Perspective = 0,
      Orthographic = 1
    };

  public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetViewportSize(uint32_t width, uint32_t height);
    void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
    ProjectionType GetProjectionType() const { return m_ProjectionType; }

    void SetOrthographic(float size, float near_clip, float far_clip);
    void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
    void SetOrthographicNearClip(float near_clip) { m_OrthographicNear = near_clip; RecalculateProjection(); }
    void SetOrthographicFarClip(float far_clip) { m_OrthographicFar = far_clip; RecalculateProjection(); }
    float GetOrthographicSize() const { return m_OrthographicSize; }
    float GetOrthographicNearClip() const { return m_OrthographicNear; }
    float GetOrthographicFarClip() const { return m_OrthographicFar; }

    void SetPerspective(float fov, float near_clip, float far_clip);
    void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
    void SetPerspectiveNearClip(float near_clip) { m_PerspectiveNear = near_clip; RecalculateProjection(); }
    void SetPerspectiveFarClip(float far_clip) { m_PerspectiveFar = far_clip; RecalculateProjection(); }
    float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
    float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
    float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

  private:
    void RecalculateProjection();

  private:
    ProjectionType m_ProjectionType = ProjectionType::Orthographic;
    float m_AspectRatio = 0.0f;

    float m_OrthographicSize = 10.0f;
    float m_OrthographicNear = -1.0f;
    float m_OrthographicFar = 1.0f;

    float m_PerspectiveFOV = glm::radians(45.0f);
    float m_PerspectiveNear = 0.01f;
    float m_PerspectiveFar = 1000.0f;
  };
}