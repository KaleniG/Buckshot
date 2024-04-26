using System;

namespace Buckshot {

  public class Entity
  {
    public readonly ulong ID;

    public Vector3 Position
    {
      get
      {
        InternalCalls.TransformComponent_GetPosition(ID, out Vector3 result);
        return result;
      }
      set
      {
        InternalCalls.TransformComponent_SetPosition(ID, ref value);
      }
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

    public Entity FindEntityByName(string name)
    {
      ulong entity_id = InternalCalls.Entity_FindEntityByName(name);

      if (entity_id != 0)
        return new Entity(entity_id);
      else
        return null;
    }

    public T As<T>() where T : Entity, new()
    {
      object instance = InternalCalls.ScriptEngine_GetScriptInstance(ID);
      return instance as T;
    }

  }
}