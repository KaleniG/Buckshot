#pragma once

#include "Buckshot/Core/Input.h"

namespace Buckshot {

  class WindowsInput : public Input
  {
  protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) override;
    virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
    virtual std::pair<float, float> GetMousePosImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
  };

}