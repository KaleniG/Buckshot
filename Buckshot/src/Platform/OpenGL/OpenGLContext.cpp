#include <bspch.h>

#include <glad/glad.h>

#include "OpenGLContext.h"

namespace Buckshot {

  OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
  {
    m_WindowHandle = windowHandle;
    BS_ASSERT(m_WindowHandle, "WindowHandle is NULL!")
  }

  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent(m_WindowHandle);
    int glad_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BS_ASSERT(glad_status, "Failed to initialize GLAD!")
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(m_WindowHandle);
  }

}