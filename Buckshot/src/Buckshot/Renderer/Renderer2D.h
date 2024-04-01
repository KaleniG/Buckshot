#pragma once

#include "Buckshot/Renderer/OrthographicCamera.h"
#include "Buckshot/Renderer/EditorCamera.h"
#include "Buckshot/Renderer/SubTexture2D.h"
#include "Buckshot/Renderer/Texture.h"
#include "Buckshot/Renderer/Camera.h"
#include "Buckshot/Scene/Components.h"

namespace Buckshot {

  class Renderer2D
  {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const OrthographicCamera& camera); // TO REMOVE
    static void Flush();
    static void EndScene();

    // RENDERING PRIMITIVES
    static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);
    static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), float thickness = 1.0f, float fade = 0.005f, int entity_id = -1);
    static void DrawCircle(const glm::mat4& transform, CircleRendererComponent& crc, int entity_id = -1);
    static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entity_id = -1);

    static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entity_id = -1);
    static void DrawLine(const glm::vec3& position_start, const glm::vec3& position_end, const glm::vec4& color, int entity_id = -1);

    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);
    static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f), int entity_id = -1);
    static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color = glm::vec4(1.0f), int entity_id = -1);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint_color = glm::vec4(1.0f));

    // IDK
    static float GetLineWidth();
    static void SetLineWidth(float width);


    // STATISTICS
    struct Statistics
    {
      uint32_t DrawCalls = 0;
      uint32_t QuadCount = 0;

      uint32_t GetTotalVertexCount() { return QuadCount * 4; }
      uint32_t GetTotalIndexCount() { return QuadCount * 6; }
    };

    static Statistics GetStats();
    static void ResetStats();

  private:
    static void FlushAndReset();
    static void StartBatch();
  };

}