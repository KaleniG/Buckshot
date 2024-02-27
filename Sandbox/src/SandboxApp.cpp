#include <Buckshot.h>
#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Buckshot::Layer
{
public:
  ExampleLayer()
    : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f)
	{
    m_Texture1 = Buckshot::Texture2D::Create("assets/textures/Checkerboard.png");
    m_Texture2 = Buckshot::Texture2D::Create("assets/textures/ChernoLogo.png");
    m_SquareVertexArray = Buckshot::VertexArray::Create();

    float verticesSquare[] =
    {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    Buckshot::Ref<Buckshot::VertexBuffer> m_SquareVertexBuffer;
    m_SquareVertexBuffer = Buckshot::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));

    {
      Buckshot::BufferLayout layout = {
            { Buckshot::ShaderDataType::Float3, "a_Position" },
            { Buckshot::ShaderDataType::Float2, "a_TexCoord" }
      };
      m_SquareVertexBuffer->SetLayout(layout);
    }

    uint32_t indicesSquare[] =
    {
      0, 1, 2, 2, 3, 0
    };

    Buckshot::Ref<Buckshot::IndexBuffer> m_IndexBufferSquare;
    m_IndexBufferSquare = Buckshot::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t));

    m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(m_IndexBufferSquare);

    auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
    //m_Shader = Buckshot::Shader::Create("assets/shaders/Texture.glsl");

    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture1", 0);
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

    auto textureShader = m_ShaderLibrary.Get("Texture");

    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(textureShader)->Bind();

    m_Texture1->Bind();
    Buckshot::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    m_Texture2->Bind();
    Buckshot::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
    ImGui::Text("Camera Position: x:%1.2f y:%1.2f\n", m_CameraPos.x, m_CameraPos.y);
    ImGui::End();
  }

private:
  Buckshot::ShaderLibrary m_ShaderLibrary;
  //Buckshot::Ref<Buckshot::Shader> m_Shader;
  Buckshot::Ref<Buckshot::VertexBuffer> m_VertexBuffer;
  Buckshot::Ref<Buckshot::IndexBuffer> m_IndexBuffer;
  Buckshot::Ref<Buckshot::VertexArray> m_SquareVertexArray;
  Buckshot::Ref<Buckshot::Texture2D> m_Texture1;
  Buckshot::Ref<Buckshot::Texture2D> m_Texture2;

  float m_MovementSpeed = 2.0f;

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
};

Buckshot::Application* Buckshot::CreateApplication()
{
  return new Sandbox;
}