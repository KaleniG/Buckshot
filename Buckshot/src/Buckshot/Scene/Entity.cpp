#include <bspch.h>

#include "Buckshot/Scene/Entity.h"

namespace Buckshot {

  Entity::Entity(entt::entity handle, Scene* scene)
  {
    m_EntityHandle = handle;
    m_Scene = scene;
  }

}