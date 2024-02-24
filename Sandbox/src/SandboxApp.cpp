#include <Buckshot.h>
#include <ImGui/imgui.h>

class ExampleLayer : public Buckshot::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f)
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

    std::shared_ptr<Buckshot::VertexBuffer> m_SquareVertexBuffer;
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

    std::shared_ptr<Buckshot::IndexBuffer> m_IndexBufferSquare;
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
    
    void main()
    {
      v_Position = a_Position;
      v_Color = a_Color;
      gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f); 
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

    m_Shader.reset(new Buckshot::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate(Buckshot::Timestep timestep) override
	{
    if (Buckshot::Input::IsKeyPressed(BS_KEY_LEFT))
      m_CameraPos.x -= 0.1f * timestep.GetSeconds();
    else if (Buckshot::Input::IsKeyPressed(BS_KEY_RIGHT))
      m_CameraPos.x += 0.1f * timestep.GetSeconds();

    if (Buckshot::Input::IsKeyPressed(BS_KEY_UP))
      m_CameraPos.y += 0.1f * timestep.GetSeconds();
    else if (Buckshot::Input::IsKeyPressed(BS_KEY_DOWN))
      m_CameraPos.y -= 0.1f * timestep.GetSeconds();

    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();

    m_Camera.SetPosition(m_CameraPos);

    Buckshot::Renderer::BeginScene(m_Camera);
    Buckshot::Renderer::Submit(m_Shader, m_SquareVertexArray);
    Buckshot::Renderer::Submit(m_Shader, m_VertexArray);
    Buckshot::Renderer::EndScene();
	}

  void OnImGuiRender() {
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
    ImGui::End();
  }

private:
  std::shared_ptr<Buckshot::Shader> m_Shader;
  std::shared_ptr<Buckshot::VertexBuffer> m_VertexBuffer;
  std::shared_ptr<Buckshot::IndexBuffer> m_IndexBuffer;
  std::shared_ptr<Buckshot::VertexArray> m_VertexArray;
  std::shared_ptr<Buckshot::VertexArray> m_SquareVertexArray;
  Buckshot::OrthographicCamera m_Camera;
  glm::vec3 m_CameraPos;
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