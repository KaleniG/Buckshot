#include <bspch.h>
#include <entt.hpp>
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

  void Scene::DestroyEntity(Entity entity)
  {
    m_Registry.destroy(entity);
  }

  void Scene::OnUpdate(Timestep timestep)
  {
    // Scripts Update
    m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
      {
        if (!nsc.Instance)
        {
          nsc.Instance = nsc.InstanciateScript();
          nsc.Instance->m_Entity = Entity(entity, this);
          nsc.Instance->OnCreate();
        }
        nsc.Instance->OnUpdate(timestep);
      }
    );

    // Renderer 2D Update
    Camera* main_camera = nullptr;
    glm::mat4 camera_transform;

    auto view = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view)
    {
      auto transform = view.get<TransformComponent>(entity);
      auto camera = view.get<CameraComponent>(entity);

      if (camera.Primary)
      {
        main_camera = &camera.Camera;
        camera_transform = transform.GetTransform();
        break;
      }
    }

    if (main_camera)
    {
      Renderer2D::BeginScene(main_camera->GetProjection(), camera_transform);

      auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
      for (auto entity : group)
      {
        auto transform = group.get<TransformComponent>(entity);
        auto sprite = group.get<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
      }

      Renderer2D::EndScene();
    }
  }

  void Scene::OnViewportResize(uint32_t width, uint32_t height)
  {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& cameraComponent = view.get<CameraComponent>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.SetViewportSize(width, height);
    }
  }

  void Scene::OnViewportResize()
  {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& cameraComponent = view.get<CameraComponent>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
  }

  Entity Scene::GetPrimaryCameraEntity()
  {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      const auto& camera = view.get<CameraComponent>(entity);
      if (camera.Primary)
        return Entity(entity, this);
    }
    return Entity();
  }

}