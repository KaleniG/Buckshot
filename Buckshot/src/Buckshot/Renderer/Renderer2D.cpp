#include <bspch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  struct Renderer2DStorage
  {
    Ref<VertexArray> vertexArray;
    Ref<Shader> textureShader;
    Ref<Texture2D> whiteTexture;
  };

  static Renderer2DStorage* s_Data;

  void Renderer2D::Init()
  {
    BS_PROFILE_FUNCTION();

    s_Data = new Renderer2DStorage;

    // TEXTURES
    
    // VERTEX BUFFER | VERTEX ARRAY | INDEX BUFFER
    s_Data->vertexArray = VertexArray::Create();

    float vertices[] =
    {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    Ref<VertexBuffer> m_VertexBuffer;
    m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

    {
      BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
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

    s_Data->whiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    s_Data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data->textureShader->Bind();
    s_Data->textureShader->SetInt("u_Texture", 0);
  }

  void Renderer2D::Shutdown()
  {
    BS_PROFILE_FUNCTION();

    delete s_Data;
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    s_Data->textureShader->Bind();
    s_Data->textureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
  }

  void Renderer2D::EndScene()
  {
    BS_PROFILE_FUNCTION();
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    BS_PROFILE_FUNCTION();

    s_Data->textureShader->SetFloat4("u_Color", color);

    s_Data->whiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
    s_Data->textureShader->SetMat4("u_Transform", transform);

    s_Data->vertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->vertexArray);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
  {
    BS_PROFILE_FUNCTION();

    s_Data->textureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
    s_Data->textureShader->SetMat4("u_Transform", transform);

    s_Data->vertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->vertexArray);
  }

}