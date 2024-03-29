#pragma once

#include <entt.hpp>

#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Renderer/EditorCamera.h"

namespace Buckshot {

  class Entity;

  class Scene
  {
  public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());
    void DestroyEntity(Entity entity);

    void OnUpdateEditor(Timestep timestep, EditorCamera& camera);
    void OnUpdateRuntime(Timestep timestep);
    void OnViewportResize(uint32_t width, uint32_t height);
    void OnViewportResize();

    Entity GetPrimaryCameraEntity();

  private:
    entt::registry m_Registry;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
  };

}