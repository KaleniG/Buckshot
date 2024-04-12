using System;
using System.Runtime.CompilerServices;

namespace Buckshot {

  public struct Vector3
  {
    public float x;
    public float y;
    public float z;

    public static Vector3 Zero = new Vector3(0.0f);

    public Vector3(float scalar)
    {
      this.x = scalar;
      this.y = scalar;
      this.z = scalar;
    }

    public Vector3(float x, float y, float z)
    {
      this.x = x;
      this.y = y;
      this.z = z;
    }

    public static Vector3 operator*(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    public static Vector3 operator +(Vector3 vector_a, Vector3 vector_b)
    {
      return new Vector3(vector_a.x + vector_b.x, vector_a.y + vector_b.y, vector_a.z + vector_b.z);
    }
  }

  public static class InternalCalls
  {
    // Input
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Input_IsKeyDown(KeyCode keycode);

    // Transform
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Entity_GetPosition(ulong entity_id, out Vector3 position);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Entity_SetPosition(ulong entity_id, ref Vector3 position);
  }

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

    public Vector3 Position
    {
      get
      {
        InternalCalls.Entity_GetPosition(ID, out Vector3 position);
        return position;
      }
      set
      {
        InternalCalls.Entity_SetPosition(ID, ref value);
      }
    }

    
  }
}