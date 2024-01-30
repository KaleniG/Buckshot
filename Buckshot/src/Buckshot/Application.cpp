#include <bspch.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Application.h"

namespace Buckshot {
  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application::Application()
  {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
  }

  Application::~Application()
  {

  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
    {
      (*--it)->OnEvent(e);
      if (e.Handled)
        break;
    }
  }

  void Application::PushLayer(Layer* layer)
  {
    m_LayerStack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* layer)
  {
    m_LayerStack.PushOverlay(layer);
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    return true;
  }

  void Application::Run()
  {
    while (m_Running)
    {
      glClearColor(1, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);
      m_Window->OnUpdate();
    }
  }

}