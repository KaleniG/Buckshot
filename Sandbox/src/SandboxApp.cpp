#include <Buckshot.h>
#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Buckshot::Layer
{
public:
  ExampleLayer()
    : Layer("Example")
	{
    m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

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

    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<Buckshot::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture1", 0);
	}

	void OnUpdate(Buckshot::Timestep timestep)
	{
    m_CameraController.OnUpdate(timestep);

    Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    Buckshot::RenderCommand::Clear();

    Buckshot::Renderer::BeginScene(m_CameraController.GetCamera());

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
    ImGui::Text("Camera Position: x:%1.2f y:%1.2f\n", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y);
    ImGui::Text("Camera ZoomLevel: %1.2f\n", m_CameraController.GetZoomLevel());
    ImGui::End();
  }

  void OnEvent(Buckshot::Event& event)
  {
    m_CameraController.OnEvent(event);
  }

private:
  Buckshot::ShaderLibrary m_ShaderLibrary;
  Buckshot::Ref<Buckshot::VertexArray> m_SquareVertexArray;
  Buckshot::Ref<Buckshot::Texture2D> m_Texture1;
  Buckshot::Ref<Buckshot::Texture2D> m_Texture2;
  Buckshot::OrthographicCameraController m_CameraController;
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