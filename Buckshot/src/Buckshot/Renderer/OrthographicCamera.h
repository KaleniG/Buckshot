#pragma once

#include <glm/glm.hpp>

namespace Buckshot {

  class OrthographicCamera
  {
  public:
    OrthographicCamera() = default;
    OrthographicCamera(float left, float right, float bottom, float top);

    inline const glm::vec3& GetPosition() const { return m_Position; }
    inline float GetRotation() const { return m_Rotation; }

    void SetPosition(const glm::vec3& position)  { m_Position = position; RicalculateViewMatrix(); }
    void SetRotation(float rotation) { m_Rotation = rotation; RicalculateViewMatrix();}
    void SetProjection(float left, float right, float bottom, float top);
  
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
  private:
    void RicalculateViewMatrix();

  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;
    glm::vec3 m_Position;

    float m_Rotation;
  };

}