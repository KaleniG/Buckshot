#pragma once

#include <GLFW/glfw3.h>

#include "Buckshot/Renderer/GraphicsContext.h"

namespace Buckshot {

  class OpenGLContext : public GraphicsContext
  {
  public:
    OpenGLContext(void* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;
  private:
    GLFWwindow* m_WindowHandle;
  };

}