#include <bspch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Buckshot/Scene/SceneCamera.h"

namespace Buckshot {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		SetProjectionType(ProjectionType::Orthographic);
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

  void SceneCamera::SetPerspective(float fov, float near_clip, float far_clip)
  {
		SetProjectionType(ProjectionType::Perspective);
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = near_clip;
		m_PerspectiveFar = far_clip;
		RecalculateProjection();
  }

  void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		switch (m_ProjectionType)
		{
		case ProjectionType::Orthographic:
		{
      float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
      float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
      float orthoBottom = -m_OrthographicSize * 0.5f;
      float orthoTop = m_OrthographicSize * 0.5f;
      m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
      break;
		}
		case ProjectionType::Perspective:
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			break;
		}
		}
	}

}