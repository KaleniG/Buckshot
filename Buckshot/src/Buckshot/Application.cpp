#include <bspch.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Application.h"
#include "Input.h"

namespace Buckshot {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::s_Instance = nullptr;

  Application::Application()
  {
    BS_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    /////////////////////////////////////////////////////////////

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    float vertices[] =
    {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    unsigned int indices[] =
    {
      0, 1, 2
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::string vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;  
    
    out vec3 v_Position;
    
    void main()
    {
      v_Position = a_Position +0.5;
      gl_Position = vec4(a_Position + 0.5, 1.0f); 
    }
    )";

    std::string fragmentSrc = R"(
    #version 330 core

    in vec3 v_Position;
    out vec4 a_Color;  
    
    void main()
    {
      a_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
    }
    )";


    m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

    /////////////////////////////////////////////////////////////
  }

  Application::~Application()
  {

  }

  void Application::PushLayer(Layer* layer)
  {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer)
  {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
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

  void Application::Run()
  {
    while (m_Running)
    {
      glClearColor(0.1f, 0.1f, 0.1f, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      m_Shader->Bind();
      glBindVertexArray(m_VAO);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

      for (Layer* layer : m_LayerStack)
        layer->OnUpdate();

      m_ImGuiLayer->Begin();
      for (Layer* layer : m_LayerStack)
        layer->OnImGuiRender();
      m_ImGuiLayer->End();

      m_Window->OnUpdate();
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e)
  {
    m_Running = false;
    BS_INFO("Closing window \"{0}\" ({1}, {2})", m_Window->GetName(), m_Window->GetWidth(), m_Window->GetHeight());
    return true;
  }

}