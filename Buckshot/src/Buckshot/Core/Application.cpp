#include <bspch.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Application.h"
#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Scripting/ScriptEngine.h"


#include <mono/jit/jit.h>

namespace Buckshot {

  Application* Application::s_Instance = nullptr;

  Application::Application(const std::string& name)
  {
    BS_PROFILE_FUNCTION();

    BS_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Window::Create(WindowProps(name));
    m_Window->SetEventCallback(BS_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();
    ScriptEngine::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
  }

  Application::~Application()
  {
    BS_PROFILE_FUNCTION();

    ScriptEngine::Shutdown();
    Renderer::Shutdown();
  }

  void Application::Close()
  {
    m_Running = false;
  }

  void Application::PushLayer(Layer* layer)
  {
    BS_PROFILE_FUNCTION();

    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* overlay)
  {
    BS_PROFILE_FUNCTION();

    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
  }

  void Application::SubmitToMainThread(const std::function<void()>& func)
  {
    std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

    m_MainThreadQueue.emplace_back(func);
  }

  void Application::OnEvent(Event& e)
  {
    BS_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BS_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BS_BIND_EVENT_FN(Application::OnWindowResize));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    {
      if (e.Handled)
        break;
      (*it)->OnEvent(e);
    }
  }

  void Application::Run()
  {
    BS_PROFILE_FUNCTION();

    while (m_Running)
    {
      BS_PROFILE_SCOPE("Application::Run() | RunLoop");

      float time = (float)glfwGetTime(); // TEMPORARY
      Timestep timestep = time - m_LastFrameTime;
      m_LastFrameTime = time;

      ExecuteMainThreadQueue();

      if (!m_Minimized)
      {
        {
          BS_PROFILE_SCOPE("Application::Run() | LayersOnUpdate");

          for (Layer* layer : m_LayerStack)
            layer->OnUpdate(timestep);
        }

        m_ImGuiLayer->Begin();

        {
          BS_PROFILE_SCOPE("Application::Run() | LayersOnImGuiRender");

          for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        }

        m_ImGuiLayer->End();
      }


      m_Window->OnUpdate();
    }
  }

  void Application::ExecuteMainThreadQueue()
  {
    std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

    for (auto& func : m_MainThreadQueue)
      func();

    m_MainThreadQueue.clear();
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    return true;
  }


  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    BS_PROFILE_FUNCTION();

    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
      m_Minimized = true;
      return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
  }

}