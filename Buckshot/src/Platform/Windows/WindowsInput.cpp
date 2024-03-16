#include <bspch.h>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Core/Application.h"

namespace Buckshot{

  bool Input::IsKeyPressed(KeyCode keycode)
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, static_cast<uint32_t>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::IsMouseButtonPressed(MouseCode button)
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
    return state == GLFW_PRESS;
  }

  std::pair<float, float> Input::GetMousePos()
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
  }

  float Input::GetMouseX()
  {
    auto[x, y] = GetMousePos();
    return x;
  }

  float Input::GetMouseY()
  {
    auto [x, y] = GetMousePos();
    return y;
  }

}