#pragma once

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Buckshot/Scene/ScriptableEntity.h"
#include "Buckshot/Scene/SceneCamera.h"

namespace Buckshot {

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

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
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

}