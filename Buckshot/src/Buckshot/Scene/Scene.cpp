#include <bspch.h>
#include <glm/glm.hpp>

#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scene/Scene.h"


namespace Buckshot {

  Scene::Scene()
  {

  }

  Scene::~Scene()
  {

  }

  entt::entity Scene::CreateEntity()
  {
    return m_Registry.create();
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