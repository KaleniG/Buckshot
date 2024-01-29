#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Buckshot {

  class BS_API Application
  {
  public:
    Application();
    virtual ~Application();
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

  private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_Running = true;
  };

  Application* CreateApplication();
}