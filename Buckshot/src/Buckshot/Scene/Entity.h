#pragma once

#include <entt.hpp>

#include "Buckshot/Core/UUID.h"
#include "Buckshot/Scene/Scene.h"
#include "Buckshot/Scene/Components.h"

namespace Buckshot {

  class Entity
  {
  public:
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;
    Entity() = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
      BS_ASSERT(!HasComponent<T>(), "Entity already has component!");
      return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& GetComponent()
    {
      BS_ASSERT(HasComponent<T>(), "Entity does not have the component!");
      return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    bool HasComponent()
    {
      return m_Scene->m_Registry.has<T>(m_EntityHandle);
    }

    template<typename T>
    void RemoveComponent()
    {
      BS_ASSERT(HasComponent<T>(), "Entity does not have the component!");
      m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    UUID GetUUID() { return GetComponent<IDComponent>().ID; }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator uint32_t() const { return (uint32_t)m_EntityHandle; }
    operator entt::entity() const { return m_EntityHandle; }

    bool operator == (const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
    bool operator != (const Entity& other) const { return !(*this == other); }

  private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;
  };

}