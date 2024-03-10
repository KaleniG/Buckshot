#include <bspch.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Application.h"
#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Renderer/Renderer.h"


namespace Buckshot {

  Application* Application::s_Instance = nullptr;

  Application::Application()
  {
    BS_PROFILE_FUNCTION();

    BS_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BS_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();
    

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
  }

  Application::~Application()
  {
    BS_PROFILE_FUNCTION();

    Renderer::Shutdown();
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

  void Application::OnEvent(Event& e)
  {
    BS_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BS_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BS_BIND_EVENT_FN(Application::OnWindowResize));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
    {
      (*it)->OnEvent(e);
      if (e.Handled)
        break;
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

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    BS_INFO("Closing window \"{0}\" ({1}, {2})", m_Window->GetName(), m_Window->GetWidth(), m_Window->GetHeight());
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