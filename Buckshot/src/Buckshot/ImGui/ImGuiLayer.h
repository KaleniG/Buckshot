#pragma once

#include "Buckshot/Layer.h"
#include "Buckshot/Events/ApplicationEvent.h"
#include "Buckshot/Events/ApplicationEvent.h"
#include "Buckshot/Events/KeyEvent.h"
#include "Buckshot/Events/MouseEvent.h"

namespace Buckshot {

  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

  private:
    float m_Time = 0.0f;
  };

}