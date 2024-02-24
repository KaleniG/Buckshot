#include <bspch.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLContext.h"

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

    BS_TRACE("OpenGL Info");
    BS_TRACE("  Vendor:        {0}", (const char*)glGetString(GL_VENDOR));
    BS_TRACE("  Renderer:      {0}", (const char*)glGetString(GL_RENDERER));
    BS_TRACE("  Version:       {0}", (const char*)glGetString(GL_VERSION));
    BS_TRACE("  GLSL Version:  {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(m_WindowHandle);
  }

}