#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

void Sandbox2D::OnAttach()
{
  // VARIABLES
  m_SquareColor = glm::vec3(0.0f);

  // CAMERA CONTROLLER
  m_CameraController = Buckshot::OrthographicCameraController(1280.0f / 720.0f);

  // VERTEX BUFFER | VERTEX ARRAY | INDEX BUFFER
  m_VertexArray = Buckshot::VertexArray::Create();

  float vertices[] =
  {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
  };

  Buckshot::Ref<Buckshot::VertexBuffer> m_VertexBuffer;
  m_VertexBuffer = Buckshot::VertexBuffer::Create(vertices, sizeof(vertices));

  {
    Buckshot::BufferLayout layout = {
          { Buckshot::ShaderDataType::Float3, "a_Position" }
    };
    m_VertexBuffer->SetLayout(layout);
  }

  uint32_t indices[] =
  {
    0, 1, 2, 2, 3, 0
  };

  Buckshot::Ref<Buckshot::IndexBuffer> m_IndexBuffer;
  m_IndexBuffer = Buckshot::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

  m_VertexArray->AddVertexBuffer(m_VertexBuffer);
  m_VertexArray->SetIndexBuffer(m_IndexBuffer);

  m_Shader = Buckshot::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Buckshot::Timestep timestep)
{
  m_CameraController.OnUpdate(timestep);

  Buckshot::RenderCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
  Buckshot::RenderCommand::Clear();

  Buckshot::Renderer::BeginScene(m_CameraController.GetCamera());

  std::dynamic_pointer_cast<Buckshot::OpenGLShader>(m_Shader)->Bind();
  std::dynamic_pointer_cast<Buckshot::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);
  Buckshot::Renderer::Submit(m_Shader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

  Buckshot::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
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
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor), ImGuiColorEditFlags_NoInputs);
  ImGui::End();
}

void Sandbox2D::OnEvent(Buckshot::Event& event)
{
  m_CameraController.OnEvent(event);
}
