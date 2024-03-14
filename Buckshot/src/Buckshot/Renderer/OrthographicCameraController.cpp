#include <bspch.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Core/KeyCodes.h"
#include "Buckshot/Core/MouseCodes.h"
#include "Buckshot/Renderer/OrthographicCameraController.h"
#include "Buckshot/Renderer/OrthographicCamera.h"

namespace Buckshot {

  OrthographicCameraController::OrthographicCameraController(float aspect_ratio)
    : m_AspectRatio(aspect_ratio)
  {
    m_ZoomLevel = 1.0f;
    m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
    m_Camera = OrthographicCamera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
    m_CameraTranslationSpeed = 2.0f;
    m_CameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  void OrthographicCameraController::OnUpdate(Timestep timestep)
  {
    BS_PROFILE_FUNCTION();

    if (Input::IsKeyPressed(BS_KEY_A))
      m_CameraPosition.x -= m_CameraTranslationSpeed * timestep.GetSeconds();
    else if (Input::IsKeyPressed(BS_KEY_D))
      m_CameraPosition.x += m_CameraTranslationSpeed * timestep.GetSeconds();

    if (Input::IsKeyPressed(BS_KEY_W))
      m_CameraPosition.y += m_CameraTranslationSpeed * timestep.GetSeconds();
    else if (Input::IsKeyPressed(BS_KEY_S))
      m_CameraPosition.y -= m_CameraTranslationSpeed * timestep.GetSeconds();

    m_Camera.SetPosition(m_CameraPosition);
    m_CameraTranslationSpeed = m_ZoomLevel;
  }

  void OrthographicCameraController::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BS_BIND_EVENT_FN(OrthographicCameraController::OnMouseButtonPressed));
    dispatcher.Dispatch<MouseScrolledEvent>(BS_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(BS_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
  }

  void OrthographicCameraController::CalculateView()
  {
    m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
    m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
  }

  bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
  {
    BS_PROFILE_FUNCTION();

    m_ZoomLevel -= event.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.10f);
    m_ZoomLevel = std::min(m_ZoomLevel, 35.0f);
    CalculateView();
    return false;
  }

  bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
  {
    BS_PROFILE_FUNCTION();

    m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
    CalculateView();
    return false;
  }

  bool OrthographicCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& event)
  {
    BS_PROFILE_FUNCTION();

    if (event.GetMouseButton() == BS_MOUSE_BUTTON_MIDDLE)
    {
      m_ZoomLevel = 1.0f;
      CalculateView();
    }
    return false;
  }

}