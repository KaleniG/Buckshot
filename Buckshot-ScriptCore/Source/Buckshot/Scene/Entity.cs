using System;

namespace Buckshot {

  public class Entity
  {
    public readonly ulong ID;

    protected Entity() 
    {
      ID = 0;
    }

    internal Entity(ulong id)
    {
      ID = id;
    }

    public bool HasComponent<T>() where T : Component, new()
    {
      Type component_type = typeof(T);
      return InternalCalls.Entity_HasComponent(ID, component_type);
    }

    public T GetComponent<T>() where T : Component, new()
    {
      if (!HasComponent<T>())
        return null;

      T component = new T() { Entity = this };
      return component;
    }
    
  }
}