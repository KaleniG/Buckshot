#include <bspch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
  {
    BS_PROFILE_FUNCTION();

    m_Position = glm::vec3(0.0f);
    m_Rotation = 0.0f;
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
  {
    BS_PROFILE_FUNCTION();

    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

  void OrthographicCamera::RicalculateViewMatrix()
  {
    BS_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
  }

}