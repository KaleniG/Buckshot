#include <bspch.h>
#include <glm/glm.hpp>

#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scene/Scene.h"

namespace Buckshot {

  Scene::Scene()
  {

  }

  Scene::~Scene()
  {

  }

  Entity Scene::CreateEntity(const std::string& name)
  {
    Entity entity = { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = (name.empty()) ? "Entity" : name;
    return entity;
  }

  void Scene::OnUpdate(Timestep timestep)
  {
    Camera* main_camera = nullptr;
    glm::mat4* camera_transform = nullptr;

    auto view = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view)
    {
      auto transform = view.get<TransformComponent>(entity);
      auto camera = view.get<CameraComponent>(entity);

      if (camera.Primary)
      {
        main_camera = &camera.Camera;
        camera_transform = &transform.Transform;
        break;
      }
    }

    if (main_camera)
    {
      Renderer2D::BeginScene(main_camera->GetProjection(), *camera_transform);

      auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
      for (auto entity : group)
      {
        auto transform = group.get<TransformComponent>(entity);
        auto sprite = group.get<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform, sprite.Color);
      }

      Renderer2D::EndScene();
    }
  }

  void Scene::OnViewportResize(uint32_t width, uint32_t height)
  {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    // Resize our non-FixedAspectRatio cameras
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& cameraComponent = view.get<CameraComponent>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.SetViewportSize(width, height);
    }

  }

}