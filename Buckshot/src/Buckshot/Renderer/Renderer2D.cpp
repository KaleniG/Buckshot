#include <bspch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Renderer/Shader.h"

namespace Buckshot {

  struct QuadVertex
  {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
  };

  struct Renderer2DData
  {
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndecies = MaxQuads * 6;

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;
  };


  static Renderer2DData s_Data;

  void Renderer2D::Init()
  {
    BS_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

    s_Data.QuadVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" }
     });

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    uint32_t* quadIndicies = new uint32_t[s_Data.MaxIndecies];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndecies; i+=6)
    {
      quadIndicies[i + 0] = offset + 0;
      quadIndicies[i + 1] = offset + 1;
      quadIndicies[i + 2] = offset + 2;

      quadIndicies[i + 3] = offset + 2;
      quadIndicies[i + 4] = offset + 3;
      quadIndicies[i + 5] = offset + 0;

      offset += 4;
    }

    Ref<IndexBuffer> quadIB;
    quadIB = IndexBuffer::Create(quadIndicies, s_Data.MaxIndecies);
    delete[] quadIndicies;


    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t WhiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));

    s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetInt("u_Texture", 0);
  }

  void Renderer2D::Shutdown()
  {
    BS_PROFILE_FUNCTION();
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
  }

  void Renderer2D::Flush()
  {
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
  }

  void Renderer2D::EndScene()
  {
    BS_PROFILE_FUNCTION();

    uint32_t data_size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, data_size);

    Flush();
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    BS_PROFILE_FUNCTION();

    s_Data.QuadVertexBufferPtr->Position = position;
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;
//     s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
//     s_Data.WhiteTexture->Bind();
// 
//     glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
//     transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
//     s_Data.TextureShader->SetMat4("u_Transform", transform);
// 
//     s_Data.QuadVertexArray->Bind();
//     RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    BS_PROFILE_FUNCTION();

    s_Data.TextureShader->SetFloat4("u_Color", tint_color);
    s_Data.TextureShader->SetFloat("u_TilingFactor", tiling_factor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
    s_Data.TextureShader->SetMat4("u_Transform", transform);

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    BS_PROFILE_FUNCTION();

    s_Data.TextureShader->SetFloat4("u_Color", color);
    s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);

    s_Data.WhiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    s_Data.TextureShader->SetMat4("u_Transform", transform);

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    BS_PROFILE_FUNCTION();

    s_Data.TextureShader->SetFloat4("u_Color", tint_color);
    s_Data.TextureShader->SetFloat("u_TilingFactor", tiling_factor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
    transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    s_Data.TextureShader->SetMat4("u_Transform", transform);

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

}