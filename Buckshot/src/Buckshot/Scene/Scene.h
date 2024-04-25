#pragma once

#include <entt.hpp>

#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Core/UUID.h"
#include "Buckshot/Renderer/EditorCamera.h"

class b2World;

namespace Buckshot {

  class Entity;

  class Scene
  {
  public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());
    Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
    Entity DuplicateEntity(Entity& entity);
    void DestroyEntity(Entity entity);

    void OnRuntimeStart();
    void OnRuntimeStop();
    void OnSimulationStart();
    void OnSimulationStop();

    void OnUpdateRuntime(Timestep timestep);
    void OnUpdateSimulation(Timestep timestep, EditorCamera& camera);
    void OnUpdateEditor(Timestep timestep, EditorCamera& camera);

    void OnViewportResize(uint32_t width, uint32_t height);
    void OnViewportResize();

    Entity GetEntityByUUID(UUID uuid);
    Entity GetPrimaryCameraEntity();

    bool IsRunning() const { return m_IsRunning; }

    static Ref<Scene> Copy(Ref<Scene> other);

    template<typename... Components>
    auto GetAllEntitiesWith()
    {
      return m_Registry.view<Components...>();
    }
  private:
    void OnPhysics2DStart();
    void OnPhysics2DStop();

    void RenderScene(EditorCamera& camera);

  private:
    bool m_IsRunning = false;

    entt::registry m_Registry;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    b2World* m_PhysicsWorld = nullptr;

    std::unordered_map<UUID, entt::entity> m_EntityMap;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
  };

}