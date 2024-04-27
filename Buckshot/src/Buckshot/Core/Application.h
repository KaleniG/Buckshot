#pragma once

#include "Buckshot/Core/Base.h"
#include "Buckshot/Core/Window.h"
#include "Buckshot/Core/LayerStack.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Buckshot {

  class Application
  {
  public:
    Application(const std::string& name = "BushApp");
    virtual ~Application();

    void Close();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void SubmitToMainThread(const std::function<void()>& func);

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    Window& GetWindow() { return *m_Window; }
    static Application& Get() { return *s_Instance; }

  private:
    void Run();
    void ExecuteMainThreadQueue();

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

  private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

    std::mutex m_MainThreadQueueMutex;
    std::vector<std::function<void()>> m_MainThreadQueue;

  private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
  };

  Application* CreateApplication();
}