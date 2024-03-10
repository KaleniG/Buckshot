#pragma once

#include "Buckshot/Core/Core.h"
#include "Buckshot/Core/Window.h"
#include "Buckshot/Core/LayerStack.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Buckshot {

  class Application
  {
  public:
    Application();
    virtual ~Application();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

  private:
    void Run();

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

  private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

  private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
  };

  Application* CreateApplication();
}