#pragma once

#include <glm/glm.hpp>

#include "Buckshot/Core/KeyCodes.h"
#include "Buckshot/Core/MouseCodes.h"

namespace Buckshot {

  class Input
  {
  public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);
    static glm::vec2 GetMousePos();

    static Scope<Input> Create();
    Input& operator=(const Input&) = delete;
  };

}