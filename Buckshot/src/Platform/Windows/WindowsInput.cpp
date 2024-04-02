#include <bspch.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Core/Application.h"

namespace Buckshot{

  bool Input::IsKeyPressed(const KeyCode keycode)
  {
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, static_cast<uint32_t>(keycode));
    return state == GLFW_PRESS;
  }

  bool Input::IsMouseButtonPressed(const MouseCode button)
  {
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
    return state == GLFW_PRESS;
  }

  glm::vec2 Input::GetMousePos()
  {
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
  }

}