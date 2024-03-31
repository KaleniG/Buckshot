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

  struct CircleVertex
  {
    glm::vec3 WorldPosition;
    glm::vec3 LocalPosition;
    glm::vec4 Color;
    float Thickness;
    float Fade;

    int EntityID;
  };

  struct LineVertex
  {
    glm::vec3 Position;
    glm::vec4 Color;

    int EntityID;
  };

  struct Renderer2DData
  {
    // CONSTANTS | QUAD
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    // RENDERING COMPONENTS | QUAD
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;
    Ref<Texture2D> WhiteTexture;

    // RENDERING COMPONENTS | CIRCLE
    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer;
    Ref<Shader> CircleShader;

    // RENDERING COMPONENTS | LINE
    Ref<VertexArray> LineVertexArray;
    Ref<VertexBuffer> LineVertexBuffer;
    Ref<Shader> LineShader;
    float LineWidth = 2.0f;

    // BATCHRENDERING | QUAD
    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;
    glm::vec4 QuadVertexPositions[4];
    
    // BATCHRENDERING | CIRCLE
    uint32_t CircleIndexCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    // BATCHRENDERING | CIRCLE
    uint32_t LineVertexCount = 0;
    LineVertex* LineVertexBufferBase = nullptr;
    LineVertex* LineVertexBufferPtr = nullptr;

    // TEXTURES | QUAD
    uint32_t TextureSlotIndex = 1; // WhiteTexture = 0
    std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;

    // STATISTICS
    Renderer2D::Statistics Stats;
  };

  static Renderer2DData s_Data;

  float Renderer2D::GetLineWidth()
  {
    return s_Data.LineWidth;
  }

  void Renderer2D::SetLineWidth(float width)
  {
    s_Data.LineWidth = width;
  }

  void Renderer2D::Init()
  {
    BS_PROFILE_FUNCTION();

    // SETTING-UP | QUAD
    s_Data.QuadVertexArray = VertexArray::Create();

    s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

    s_Data.QuadVertexBuffer->SetLayout
    ({
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

    // SETTING-UP | CIRCLE

    s_Data.CircleVertexArray = VertexArray::Create();

    s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

    s_Data.CircleVertexBuffer->SetLayout
    ({
      { ShaderDataType::Float3, "a_WorldPosition" },
      { ShaderDataType::Float3, "a_LocalPosition" },
      { ShaderDataType::Float4, "a_Color" },
      { ShaderDataType::Float, "a_Thickness" },
      { ShaderDataType::Float, "a_Fade" },
      { ShaderDataType::Int, "a_EntityID" }
    });
    s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);

    s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Intentional (PS: Maybe rename the variable)

    s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

    // SETTING-UP | LINE

    s_Data.LineVertexArray = VertexArray::Create();

    s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));

    s_Data.LineVertexBuffer->SetLayout
    ({
      { ShaderDataType::Float3, "a_Position" },
      { ShaderDataType::Float4, "a_Color" },
      { ShaderDataType::Int, "a_EntityID" }
    });
    s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

    s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

    // SHADER | QUAD

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
      samplers[i] = i;

    s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
    s_Data.QuadShader->Bind();
    s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    // SHADER | CIRCLE

    s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");

    // SHADER | LINE

    s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");

    // OTHER | QUAD | CIRCLE | LINE

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

    s_Data.QuadShader->Bind();
    s_Data.QuadShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.CircleShader->Bind();
    s_Data.CircleShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.LineShader->Bind();
    s_Data.LineShader->SetMat4("u_ViewProjection", view_projection);

    StartBatch();
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    // TODO: Eliminate
    BS_PROFILE_FUNCTION();

    s_Data.QuadShader->Bind();
    s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    StartBatch();
  }

  void Renderer2D::BeginScene(const EditorCamera& camera)
  {
    BS_PROFILE_FUNCTION();

    glm::mat4 view_projection = camera.GetViewProjection();

    s_Data.QuadShader->Bind();
    s_Data.QuadShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.CircleShader->Bind();
    s_Data.CircleShader->SetMat4("u_ViewProjection", view_projection);

    s_Data.LineShader->Bind();
    s_Data.LineShader->SetMat4("u_ViewProjection", view_projection);

    StartBatch();
  }

  void Renderer2D::EndScene()
  {
    BS_PROFILE_FUNCTION();

    Flush();
  }

  void Renderer2D::Flush()
  {
    BS_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
      s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

      for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        s_Data.TextureSlots[i]->Bind(i);

      s_Data.QuadShader->Bind();
      RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
      s_Data.Stats.DrawCalls++;
    }

    if (s_Data.CircleIndexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
      s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

      s_Data.CircleShader->Bind();
      RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
      s_Data.Stats.DrawCalls++;
    }

    if (s_Data.LineVertexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
      s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

      s_Data.LineShader->Bind();
      RenderCommand::SetLineWidth(s_Data.LineWidth);
      RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
      s_Data.Stats.DrawCalls++;
    }
  }

  void Renderer2D::FlushAndReset()
  {
    EndScene();
    StartBatch();
  }

  void Renderer2D::StartBatch()
  {
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.CircleIndexCount = 0;
    s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

    s_Data.LineVertexCount = 0;
    s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::DrawLine(const glm::vec3& position_start, const glm::vec3& position_end, const glm::vec4& color, int entity_id)
  {
    s_Data.LineVertexBufferPtr->Position = position_start;
    s_Data.LineVertexBufferPtr->Color = color;
    s_Data.LineVertexBufferPtr->EntityID = entity_id;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexBufferPtr->Position = position_end;
    s_Data.LineVertexBufferPtr->Color = color;
    s_Data.LineVertexBufferPtr->EntityID = entity_id;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexCount += 2;
  }

  void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entity_id)
  {
    glm::vec3 p0 = glm::vec3(position.x - size.x / 2.0f, position.y - size.y / 2.0f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x / 2.0f, position.y - size.y / 2.0f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x / 2.0f, position.y + size.y / 2.0f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x / 2.0f, position.y + size.y / 2.0f, position.z);

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
  }

  void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entity_id)
  {
    glm::vec3 line_vertices[4];

    for (size_t i = 0; i < 4; i++)
    {
      line_vertices[i] = transform * s_Data.QuadVertexPositions[i];
    }

    DrawLine(line_vertices[0], line_vertices[1], color);
    DrawLine(line_vertices[1], line_vertices[2], color);
    DrawLine(line_vertices[2], line_vertices[3], color);
    DrawLine(line_vertices[3], line_vertices[0], color);
  }

  void Renderer2D::DrawCircle(const glm::mat4& transform, CircleRendererComponent& crc, int entity_id)
  {
    //if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
    //  FlushAndReset();

    for (size_t i = 0; i < 4; i++)
    {
      s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
      s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
      s_Data.CircleVertexBufferPtr->Color = crc.Color;
      s_Data.CircleVertexBufferPtr->Thickness = crc.Thickness;
      s_Data.CircleVertexBufferPtr->Fade = crc.Fade;
      s_Data.CircleVertexBufferPtr->EntityID = entity_id;
      s_Data.CircleVertexBufferPtr++;
    }

    s_Data.CircleIndexCount += 6;

    s_Data.Stats.QuadCount++; // Stupidass name TO-CHANGE
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