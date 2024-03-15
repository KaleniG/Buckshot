#include <bspch.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLContext.h"

namespace Buckshot {

  OpenGLContext::OpenGLContext(void* windowHandle)
  {
    m_WindowHandle = static_cast<GLFWwindow*>(windowHandle);
    BS_ASSERT(m_WindowHandle, "WindowHandle is NULL!");
  }

  void OpenGLContext::Init()
  {
    BS_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);
    int glad_status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BS_ASSERT(glad_status, "Failed to initialize GLAD!");
  }

  void OpenGLContext::SwapBuffers()
  {
    BS_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
  }

}