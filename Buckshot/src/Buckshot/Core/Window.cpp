#include <bspch.h>

#include "Buckshot/Core/Window.h"

#ifdef BS_PLATFORM_WINDOWS
  #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Buckshot
{

  Scope<Window> Window::Create(const WindowProps& props)
  {
    #ifdef BS_PLATFORM_WINDOWS
      return CreateScope<WindowsWindow>(props);
    #else
      BS_ASSERT(false, "Unknown platform!");
      return nullptr;
    #endif
  }

}