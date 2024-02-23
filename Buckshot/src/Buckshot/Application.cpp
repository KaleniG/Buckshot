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
    m_VertexArray.reset(VertexArray::Create());

    float vertices[] =
    {
      -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.3f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.4f, 0.5f, 0.6f, 1.0f,
       0.0f,  0.5f, 0.0f, 0.7f, 0.8f, 0.9f, 1.0f
    };

    m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    m_VertexBuffer->Bind();

    {
      BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
      };
      m_VertexBuffer->SetLayout(layout);
    }

    uint32_t indices[] =
    {
      0, 1, 2
    };

    m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    //////////////////////////////////////////

    m_SquareVertexArray.reset(VertexArray::Create());

    float verticesSquare[] =
    {
      -0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.4f, 0.4f, 0.4f, 1.0f,
       0.5f,  0.5f, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.9f, 0.9f, 0.9f, 1.0f
    };

    std::shared_ptr<VertexBuffer> m_SquareVertexBuffer;
    m_SquareVertexBuffer.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

    {
      BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
      };
      m_SquareVertexBuffer->SetLayout(layout);
    }

    uint32_t indicesSquare[] =
    {
      0, 1, 2, 2, 3, 0
    };

    std::shared_ptr<IndexBuffer> m_IndexBufferSquare;
    m_IndexBufferSquare.reset(IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

    m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(m_IndexBufferSquare);


    std::string vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;   
    layout (location = 1) in vec4 a_Color;     
    
    out vec3 v_Position;
    out vec4 v_Color;
    
    void main()
    {
      v_Position = a_Position;
      v_Color = a_Color;
      gl_Position = vec4(a_Position, 1.0f); 
    }
    )";

    std::string fragmentSrc = R"(
    #version 330 core

    in vec3 v_Position;
    in vec4 v_Color;

    out vec4 a_Color;  
    
    void main()
    {
      a_Color = v_Color;
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
      m_SquareVertexArray->Bind();
      glDrawElements(GL_TRIANGLES, m_SquareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
      m_VertexArray->Bind();
      glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);



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