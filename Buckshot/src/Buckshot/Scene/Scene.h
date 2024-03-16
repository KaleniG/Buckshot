#pragma once

#include <entt.hpp>

#include "Buckshot/Core/Timestep.h"

namespace Buckshot {

  class Scene
  {
  public:
    Scene();
    ~Scene();

    entt::entity CreateEntity();

    entt::registry& GetRegistry() { return m_Registry; }

    void OnUpdate(Timestep timestep);

  private:
    entt::registry m_Registry;
  };

}