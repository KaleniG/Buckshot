namespace Buckshot
{
  public abstract class Component
  {
    public Entity Entity { get; internal set; }
  }

  public class TransformComponent : Component
  {
    public Vector3 Position
    {
      get
      {
        InternalCalls.TransformComponent_GetPosition(Entity.ID, out Vector3 position);
        return position;
      }
      set
      {
        InternalCalls.TransformComponent_SetPosition(Entity.ID, ref value);
      }
    }

    public Vector3 Rotation
    {
      get
      {
        InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
        return rotation;
      }
      set
      {
        InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
      }
    }

    public Vector3 Scale
    {
      get
      {
        InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
        return scale;
      }
      set
      {
        InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
      }
    }
  }

  public class Rigidbody2DComponent : Component
  {
    public void ApplyLinearImpulse(Vector2 impulse, Vector2 world_position, bool wake)
    {
      InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref world_position, wake);
    }

    public void ApplyLinearImpulse(Vector2 impulse, bool wake)
    {
      InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
    }
  }
}
