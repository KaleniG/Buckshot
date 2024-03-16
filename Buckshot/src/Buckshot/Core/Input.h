#pragma once

#include "Buckshot/Core/Base.h"
#include "Buckshot/Core/KeyCodes.h"
#include "Buckshot/Core/MouseCodes.h"

namespace Buckshot {

  class Input
  {
  public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);
    static std::pair<float, float> GetMousePos();
    static float GetMouseX();
    static float GetMouseY();

    static Scope<Input> Create();
    Input& operator=(const Input&) = delete;
  };

}