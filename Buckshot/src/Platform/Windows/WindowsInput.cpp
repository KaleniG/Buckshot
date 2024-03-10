#include <bspch.h>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Application.h"
#include "Platform/Windows/WindowsInput.h"

namespace Buckshot{

  bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, static_cast<uint32_t>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
    return state == GLFW_PRESS;
  }

  std::pair<float, float> WindowsInput::GetMousePosImpl()
  {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
  }

  float WindowsInput::GetMouseXImpl()
  {
    auto[x, y] = GetMousePosImpl();
    return x;
  }

  float WindowsInput::GetMouseYImpl()
  {
    auto [x, y] = GetMousePosImpl();
    return y;
  }

}