#pragma once

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Buckshot/Core/UUID.h"
#include "Buckshot/Renderer/Texture.h"
#include "Buckshot/Scene/SceneCamera.h"

namespace Buckshot {

  struct IDComponent
  {
    UUID ID;

    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
    IDComponent(const UUID& id) : ID(id) {}
  };

  struct TagComponent
  {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) : Tag(tag) {}
  };

  struct TransformComponent
  {
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position) : Position(position) {}

    glm::mat4 GetTransform() const
    {
      glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

      return glm::translate(glm::mat4(1.0f), Position)
        * rotation
        * glm::scale(glm::mat4(1.0f), Scale);
    }
  };

  struct SpriteRendererComponent
  {
    glm::vec4 Color = glm::vec4(1.0f);
    Ref<Texture2D> Texture;
    float TilingFactor = 1.0f;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
  };

  struct CircleRendererComponent
  {
    glm::vec4 Color = glm::vec4(1.0f);
    float Thickness = 1.0f;
    float Fade = 0.005f;

    CircleRendererComponent() = default;
    CircleRendererComponent(const CircleRendererComponent&) = default;
  };

  struct CameraComponent
  {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const SceneCamera& camera) : Camera(camera) {}
  };

  class ScriptableEntity;
  struct NativeScriptComponent
  {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstanciateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template<typename T>
    void Bind()
    {
      InstanciateScript = []() { return static_cast<ScriptableEntity*>(new T); };
      DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    }
  };

  struct ScriptComponent
  {
    std::string Name;

    ScriptComponent() = default;
    ScriptComponent(const ScriptComponent&) = default;
  };

  struct Rigidbody2DComponent
  {
    enum class BodyType
    {
      Static,
      Dynamic,
      Kinematic
    };

    BodyType Type = BodyType::Static;
    bool FixedRotation = false;
    void* RuntimeBody = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
  };

  struct BoxCollider2DComponent
  {
    glm::vec2 Offset = glm::vec2(0.0f);
    glm::vec2 Size = glm::vec2(0.5f);

    float Density = 1.0f;
    float Friction = 0.5f;
    float Restituition = 0.0f;
    float RestituitionThreshold = 0.5f;

    void* RuntimeFixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
  };

  struct CircleCollider2DComponent
  {
    glm::vec2 Offset = glm::vec2(0.0f);
    float Radius = 0.5f;
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restituition = 0.0f;
    float RestituitionThreshold = 0.5f;

    void* RuntimeFixture = nullptr;

    CircleCollider2DComponent() = default;
    CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
  };

  template<typename... Component>
  struct ComponentGroup {};

  using AllComponents = ComponentGroup
  <
    TransformComponent, SpriteRendererComponent, CircleRendererComponent, 
    CameraComponent, NativeScriptComponent, ScriptComponent,
    Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent
  >;

}