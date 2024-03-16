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
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group)
    {
      auto transform = group.get<TransformComponent>(entity);
      auto sprite = group.get<SpriteRendererComponent>(entity);

      Renderer2D::DrawQuad(transform, sprite.Color);
    }
  }

}