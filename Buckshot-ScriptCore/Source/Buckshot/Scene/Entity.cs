using System;

namespace Buckshot {

  public class Entity
  {
    public readonly ulong ID;
    public string Name
    {
      get
      {
        return InternalCalls.Entity_GetName(ID);
      }
      set { }
    }


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

    public T As<T>() where T : Entity, new()
    {
      object instance = InternalCalls.ScriptEngine_GetScriptInstance(ID);
      return instance as T;
    }

    public Entity FindEntityByName(string name)
    {
      ulong entity_id = InternalCalls.Entity_FindEntityByName(name);

      if (entity_id != 0)
        return new Entity(entity_id);
      else
        return null;
    }
  }
}