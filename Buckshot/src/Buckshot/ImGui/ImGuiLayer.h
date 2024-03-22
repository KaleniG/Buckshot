#pragma once

#include "Buckshot/Core/Layer.h"
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
    virtual void OnEvent(Event& e) override;

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }
    void SetDarkThemeColors();

  private:
    bool m_BlockEvents = false;
  };

}