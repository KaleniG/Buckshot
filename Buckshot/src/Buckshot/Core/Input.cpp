#include <bspch.h>

#include "Buckshot/Core/Input.h"

#ifdef BS_PLATFORM_WINDOWS
  #include "Platform/Windows/WindowsInput.h"
#endif

namespace Buckshot {

  Scope<Input> Input::s_Instance = Input::Create();

  Scope<Input> Input::Create()
  {
    #ifdef BS_PLATFORM_WINDOWS
      return CreateScope<WindowsInput>();
    #else
      BS_ASSERT(false, "Unknown platform!");
      return nullptr;
    #endif
  }
}