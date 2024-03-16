#pragma once

#include <entt.hpp>

#include "Buckshot/Core/Timestep.h"

namespace Buckshot {

  class Entity;

  class Scene
  {
  public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());

    void OnUpdate(Timestep timestep);

  private:
    entt::registry m_Registry;

    friend class Entity;
  };

}