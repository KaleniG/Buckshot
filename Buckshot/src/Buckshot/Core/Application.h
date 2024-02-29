#pragma once

#include "Buckshot/Core/Core.h"
#include "Buckshot/Core/Window.h"
#include "Buckshot/Core/LayerStack.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/ImGui/ImGuiLayer.h"

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
    bool OnWindowResize(WindowResizeEvent& e);

  private:
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

  private:
    static Application* s_Instance;
  };

  Application* CreateApplication();
}