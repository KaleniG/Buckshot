#include <bspch.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  struct Renderer2DStorage
  {
    Ref<VertexArray> vertexArray;
    Ref<Shader> flatColorShader;
  };

  static Renderer2DStorage* s_Data;

  void Renderer2D::Init()
  {
    s_Data = new Renderer2DStorage;

    // VERTEX BUFFER | VERTEX ARRAY | INDEX BUFFER
    s_Data->vertexArray = VertexArray::Create();

    float vertices[] =
    {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.5f,  0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f
    };

    Ref<VertexBuffer> m_VertexBuffer;
    m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

    {
      BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" }
      };
      m_VertexBuffer->SetLayout(layout);
    }

    uint32_t indices[] =
    {
      0, 1, 2, 2, 3, 0
    };

    Ref<IndexBuffer> m_IndexBuffer;
    m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    s_Data->vertexArray->AddVertexBuffer(m_VertexBuffer);
    s_Data->vertexArray->SetIndexBuffer(m_IndexBuffer);

    s_Data->flatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
  }

  void Renderer2D::Shutdown()
  {
    delete s_Data;
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));

  }

  void Renderer2D::EndScene()
  {

  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColorShader)->UploadUniformFloat4("u_Color", color);

    s_Data->vertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->vertexArray);
  }

}