#pragma once

#include "Buckshot/Core/Base.h"
#include "Buckshot/Core/Window.h"
#include "Buckshot/Core/LayerStack.h"
#include "Buckshot/Events/Event.h"
#include "Buckshot/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Buckshot {

  struct ApplicationCommandLineArgs
  {
    int Count = 0;
    char** Args = nullptr;

    const char* operator[](int index) const
    {
      BS_ASSERT(index < Count);
      return Args[index];
    }
  };

  struct ApplicationSpecification
  {
    std::string Name = "BushApp";
    std::string WorkingDirectory;
    ApplicationCommandLineArgs CommandLineArgs;
  };

  class Application
  {
  public:
    Application(const ApplicationSpecification& specification);
    virtual ~Application();

    void Close();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void SubmitToMainThread(const std::function<void()>& func);

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    Window& GetWindow() { return *m_Window; }
    static Application& Get() { return *s_Instance; }
    const ApplicationSpecification& GetSpecification() const { return m_Specification; }

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
    ApplicationSpecification m_Specification;

    std::mutex m_MainThreadQueueMutex;
    std::vector<std::function<void()>> m_MainThreadQueue;

  private:
    static Application* s_Instance;
    friend int ::main(int argc, char** argv);
  };

  Application* CreateApplication(ApplicationCommandLineArgs args);
}