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
    float TexIndex;
    float TilingFactor;

    int EntityID;
  };

  struct Renderer2DData
  {
    // CONSTANTS
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    // RENDERING COMPONENTS
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    // BATCHRENDERING
    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;
    glm::vec4 QuadVertexPositions[4];

    uint32_t TextureSlotIndex = 1; // WhiteTexture = 0
    std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

    // STATISTICS
    Renderer2D::Statistics Stats;
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
      { ShaderDataType::Float2, "a_TexCoord" },
      { ShaderDataType::Float, "a_TexIndex" },
      { ShaderDataType::Float, "a_TilingFactor" },
      { ShaderDataType::Int, "a_EntityID" }
    });
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
    {
      quadIndices[i + 0] = offset + 0;
      quadIndices[i + 1] = offset + 1;
      quadIndices[i + 2] = offset + 2;
      quadIndices[i + 3] = offset + 2;
      quadIndices[i + 4] = offset + 3;
      quadIndices[i + 5] = offset + 0;
      offset += 4;
    }

    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
      samplers[i] = i;

    s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    s_Data.QuadVertexPositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
    s_Data.QuadVertexPositions[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
    s_Data.QuadVertexPositions[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
    s_Data.QuadVertexPositions[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);
  }

  void Renderer2D::Shutdown()
  {
    BS_PROFILE_FUNCTION();

    delete[] s_Data.QuadVertexBufferBase;
  }

  void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
  {
    BS_PROFILE_FUNCTION();

    glm::mat4 view_projection = camera.GetProjection() * glm::inverse(transform);

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::BeginScene(const EditorCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    glm::mat4 view_projection = camera.GetViewProjection();

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::EndScene()
  {
    BS_PROFILE_FUNCTION();

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
    Flush();
  }

  void Renderer2D::Flush()
  {
    BS_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount == 0)
      return;

    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
      s_Data.TextureSlots[i]->Bind(i);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
  }

  void Renderer2D::FlushAndReset()
  {
    EndScene();

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entity_id)
  {
    if (src.Texture)
      DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entity_id);
    else
      DrawQuad(transform, src.Color, entity_id);
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entity_id)
  {
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    const float textureIndex = 0.0f; // White Texture
    const float tilingFactor = 1.0f;

    if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
      FlushAndReset();

    for (size_t i = 0; i < quadVertexCount; i++)
    {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = color;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr->EntityID = entity_id;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor /*= 1.0f*/, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/, int entity_id)
  {
    BS_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
      if (*s_Data.TextureSlots[i] == *texture)
      {
        textureIndex = (float)i;
        break;
      }
    }

    if (textureIndex == 0.0f)
    {
      if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

      textureIndex = (float)s_Data.TextureSlotIndex;
      s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
      s_Data.TextureSlotIndex++;
    }

    for (size_t i = 0; i < quadVertexCount; i++)
    {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = tint_color;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
      s_Data.QuadVertexBufferPtr->EntityID = entity_id;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/, int entity_id)
  {
    BS_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    constexpr float tiling_factor = 1.0f;
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    const glm::vec2* textureCoords = subtexture->GetTextureCoords();
    const Ref<Texture2D> texture = subtexture->GetTexture();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
      if (*s_Data.TextureSlots[i] == *texture)
      {
        textureIndex = (float)i;
        break;
      }
    }

    if (textureIndex == 0.0f)
    {
      if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

      textureIndex = (float)s_Data.TextureSlotIndex;
      s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
      s_Data.TextureSlotIndex++;
    }

    for (size_t i = 0; i < quadVertexCount; i++)
    {
      s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = tint_color;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
      s_Data.QuadVertexBufferPtr->EntityID = entity_id;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    glm::mat4 transform = 
      glm::translate(glm::mat4(1.0f), position) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, color);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    glm::mat4 transform = 
      glm::translate(glm::mat4(1.0f), position) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tint_color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/)
  {
    glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, subtexture, tint_color);
  }
  
  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    glm::mat4 transform = 
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
  {
    glm::mat4 transform = 
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tiling_factor, tint_color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tint_color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color /*= glm::vec4(1.0f)*/)
  {
    glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, subtexture, tint_color);
  }

  Renderer2D::Statistics Renderer2D::GetStats()
  {
    return s_Data.Stats;
  }

  void Renderer2D::ResetStats()
  {
    memset(&s_Data.Stats, 0, sizeof(Statistics));
  }

}