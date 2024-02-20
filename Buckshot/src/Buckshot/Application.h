#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Buckshot {

  class Application
  {
  public:
    Application();
    virtual ~Application();
    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

  private:
    bool OnWindowClose(WindowCloseEvent& e);

  private:
    static Application* s_Instance;
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    bool m_Running = true;
  };

  Application* CreateApplication();
}