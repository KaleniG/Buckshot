#pragma once

#include "Buckshot/Layer.h"

namespace Buckshot {

  class BS_API ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnUpdate(Event& event);

  private:
    float m_Time = 0.0f;
  };

}