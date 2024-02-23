#include <bspch.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Application.h"
#include "Input.h"

namespace Buckshot {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::s_Instance = nullptr;

  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::None:    break;
    case ShaderDataType::Bool:    return GL_BOOL;
    case ShaderDataType::Float:   return GL_FLOAT;
    case ShaderDataType::Float2:  return GL_FLOAT;
    case ShaderDataType::Float3:  return GL_FLOAT;
    case ShaderDataType::Float4:  return GL_FLOAT;
    case ShaderDataType::Int:     return GL_INT;
    case ShaderDataType::Int2:    return GL_INT;
    case ShaderDataType::Int3:    return GL_INT;
    case ShaderDataType::Int4:    return GL_INT;
    case ShaderDataType::Mat3:    return GL_FLOAT;
    case ShaderDataType::Mat4:    return GL_FLOAT;
    }

    BS_ASSERT(false, "Unknown ShaderDataType specified")
      return 0;
  }

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
      -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.3f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.4f, 0.5f, 0.6f, 1.0f,
       0.0f,  0.5f, 0.0f, 0.7f, 0.8f, 0.9f, 1.0f
    };

    uint32_t indices[] =
    {
      0, 1, 2
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

    uint32_t index = 0;
    const auto& layout = m_VertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(
        index, 
        element.GetComponentCount(), 
        ShaderDataTypeToOpenGLBaseType(element.DataType), 
        (element.Normalized) ? GL_FALSE : GL_TRUE, 
        layout.GetStride(),
        (const void*)element.Offset);
      index++;
    }


    m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    std::string vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;   
    layout (location = 1) in vec4 a_Color;     
    
    out vec3 v_Position;
    out vec4 v_Color;
    
    void main()
    {
      v_Position = a_Position +0.5;
      v_Color = a_Color;
      gl_Position = vec4(a_Position + 0.5, 1.0f); 
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
      glBindVertexArray(m_VAO);
      glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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