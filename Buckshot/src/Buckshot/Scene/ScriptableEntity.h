#pragma once

#include "Buckshot/Scene/Entity.h"

namespace Buckshot {

  class ScriptableEntity
  {
  public:
    template<typename T>
    T& GetComponent()
    {
      return m_Entity.GetComponent<T>();
    }

  private:
    Entity m_Entity;
    friend class Scene;
  };

}