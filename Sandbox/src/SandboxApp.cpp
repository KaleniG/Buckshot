#include <Buckshot.h>
#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Buckshot::Layer
{
public:
  ExampleLayer()
    : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f), m_Color1(1.0f, 0.2f, 0.2f, 1.0f), m_Color2(0.2f, 0.2f, 1.0f, 1.0f)
	{
    m_VertexArray.reset(Buckshot::VertexArray::Create());

    float vertices[] =
    {
      -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.3f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.4f, 0.5f, 0.6f, 1.0f,
       0.0f,  0.5f, 0.0f, 0.7f, 0.8f, 0.9f, 1.0f
    };

    m_VertexBuffer.reset(Buckshot::VertexBuffer::Create(vertices, sizeof(vertices)));
    m_VertexBuffer->Bind();

    {
      Buckshot::BufferLayout layout = {
            { Buckshot::ShaderDataType::Float3, "a_Position" },
            { Buckshot::ShaderDataType::Float4, "a_Color" }
      };
      m_VertexBuffer->SetLayout(layout);
    }

    uint32_t indices[] =
    {
      0, 1, 2
    };

    m_IndexBuffer.reset(Buckshot::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_SquareVertexArray.reset(Buckshot::VertexArray::Create());

    float verticesSquare[] =
    {
      -0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.4f, 0.4f, 0.4f, 1.0f,
       0.5f,  0.5f, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.9f, 0.9f, 0.9f, 1.0f
    };

    Buckshot::Ref<Buckshot::VertexBuffer> m_SquareVertexBuffer;
    m_SquareVertexBuffer.reset(Buckshot::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

    {
      Buckshot::BufferLayout layout = {
            { Buckshot::ShaderDataType::Float3, "a_Position" },
            { Buckshot::ShaderDataType::Float4, "a_Color" }
      };
      m_SquareVertexBuffer->SetLayout(layout);
    }

    uint32_t indicesSquare[] =
    {
      0, 1, 2, 2, 3, 0
    };

    Buckshot::Ref<Buckshot::IndexBuffer> m_IndexBufferSquare;
    m_IndexBufferSquare.reset(Buckshot::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

    m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(m_IndexBufferSquare);

    std::string vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;   
    layout (location = 1) in vec4 a_Color;     
    
    out vec3 v_Position;
    out vec4 v_Color;

    uniform mat4 u_ViewProjectionMatrix;
    uniform mat4 u_Transform;
    
    void main()
    {
      v_Position = a_Position;
      v_Color = a_Color;
      gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f); 
    }
    )";

    std::string fragmentSrc = R"(
    #version 330 core

    in vec3 v_Position;
    in vec4 v_Color;

    out vec4 a_Color;  
    
    uniform vec4 u_Color;

    void main()
    {
      a_Color = u_Color;
    }
    )";

    m_Shader.reset(Buckshot::Shader::Create(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Buckshot::Timestep timestep) override
	{
    if (Buckshot::Input::IsKeyPressed(BS_KEY_LEFT))
      m_CameraPos.x -= m_MovementSpeed * timestep.GetSeconds();
    else if (Buckshot::Input::IsKeyPressed(BS_KEY_RIGHT))
      m_CameraPos.x += m_MovementSpeed * timestep.GetSeconds();

    if (Buckshot::Input::IsKeyPressed(BS_KEY_UP))
      m_CameraPos.y += m_MovementSpeed * timestep.GetSeconds();
    else if (Buckshot::Input::IsKeyPressed(BS_KEY_DOWN))
      m_CameraPos.y -= m_MovementSpeed * timestep.GetSeconds();

    m_Camera.SetPosition(m_CameraPos);

    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();
    Buckshot::Renderer::BeginScene(m_Camera);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(m_Shader)->Bind();

    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        glm::vec3 shapesPos(i * 0.11f, j * 0.11f, 0.0f);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), shapesPos) * scale;

        if (i % 2 == 0)
          std::dynamic_pointer_cast<Buckshot::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_Color1);
        else
          std::dynamic_pointer_cast<Buckshot::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_Color2);

        Buckshot::Renderer::Submit(m_Shader, m_SquareVertexArray, transform);
        Buckshot::Renderer::Submit(m_Shader, m_VertexArray, transform);
      }
    }

    Buckshot::Renderer::EndScene();
	}

  void OnImGuiRender() {
    float color1[3] = { m_Color1.r, m_Color1.g, m_Color1.b};
    float color2[3] = { m_Color2.r, m_Color2.g, m_Color2.b};

    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f));
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoFocusOnAppearing | 
      ImGuiWindowFlags_NoSavedSettings | 
      ImGuiWindowFlags_NoDecoration | 
      ImGuiWindowFlags_AlwaysAutoResize | 
      ImGuiWindowFlags_NoDocking | 
      ImGuiWindowFlags_NoMove | 
      ImGuiWindowFlags_NoNav;
    ImGui::Begin("Overlay", (bool*)true, window_flags);
    ImGui::Text("Camera Position: %1.2f, %1.2f\n", m_CameraPos.x, m_CameraPos.y);
    ImGui::Separator();
    ImGuiColorEditFlags coloredit_flags = ImGuiColorEditFlags_NoInputs;
    ImGui::ColorEdit3("Color 1", color1, coloredit_flags);
    ImGui::ColorEdit3("Color 2", color2, coloredit_flags);
    ImGui::End();

    m_Color1 = glm::vec4(color1[0], color1[1], color1[2], 1.0f);
    m_Color2 = glm::vec4(color2[0], color2[1], color2[2], 1.0f);
  }

private:
  Buckshot::Ref<Buckshot::Shader> m_Shader;
  Buckshot::Ref<Buckshot::VertexBuffer> m_VertexBuffer;
  Buckshot::Ref<Buckshot::IndexBuffer> m_IndexBuffer;
  Buckshot::Ref<Buckshot::VertexArray> m_VertexArray;
  Buckshot::Ref<Buckshot::VertexArray> m_SquareVertexArray;

  float m_MovementSpeed = 2.0f;

  Buckshot::OrthographicCamera m_Camera;
  glm::vec3 m_CameraPos;

  glm::vec4 m_Color1;
  glm::vec4 m_Color2;
};

class Sandbox : public Buckshot::Application
{
public:
  Sandbox()
  {
		PushLayer(new ExampleLayer());
  }

  ~Sandbox()
  {

  }


};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}