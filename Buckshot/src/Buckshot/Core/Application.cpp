#include <bspch.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Buckshot/Core/Application.h"
#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Renderer/Renderer.h"

namespace Buckshot {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::s_Instance = nullptr;

  Application::Application()
  {
    BS_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
  }

  Application::~Application()
  {

  }

  void Application::PushLayer(Layer* layer)
  {
    m_LayerStack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* layer)
  {
    m_LayerStack.PushOverlay(layer);
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
    {
      (*--it)->OnEvent(e);
      if (e.Handled)
        break;
    }
  }

  void Application::Run()
  {
    while (m_Running)
    {
      float time = (float)glfwGetTime(); // TEMPORARY
      Timestep timestep = time - m_LastFrameTime;
      m_LastFrameTime = time;

      if (!m_Minimized)
      {
        for (Layer* layer : m_LayerStack)
          layer->OnUpdate(timestep);

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
          layer->OnImGuiRender();
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