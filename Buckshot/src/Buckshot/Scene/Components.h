#pragma once

#include "Buckshot/Renderer/Camera.h"

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
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform) : Transform(transform) {}

    operator glm::mat4& () { return Transform; }
    operator const glm::mat4& () const { return Transform; }
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
    Buckshot::Camera Camera;
    bool Primary = true;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Buckshot::Camera& camera) : Camera(camera) {}
    CameraComponent(const glm::mat4& projection) : Camera(projection) {}
  };

}