using System;

namespace Buckshot
{
  public abstract class Component
  {
    public Entity Entity { get; internal set; }
  }

  public class Transform : Component
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

  public class Rigidbody2D : Component
  {
    public class Velocity2D
    {
      public float x;
      public float y;

      public Velocity2D(Vector2 vec2)
      {
        this.x = vec2.x;
        this.y = vec2.y;
      }

      public float Length()
      {
        return (float)Math.Sqrt(x * x + y * y);
      }
    }

    public enum BodyType2D
    {
      Static,
      Dynamic,
      Kinematic
    }

    public BodyType2D Type
    {
      get
      {
        return InternalCalls.Rigidbody2DComponent_GetBodyType(Entity.ID);
      }
      set
      {
        InternalCalls.Rigidbody2DComponent_SetBodyType(Entity.ID, ref value);
      }
    }

    public bool FixedRotation
    {
      get
      {
        return InternalCalls.Rigidbody2DComponent_GetFixedRotation(Entity.ID);
      }
      set
      {
        InternalCalls.Rigidbody2DComponent_SetFixedRotation(Entity.ID, ref value);
      }
    }

    public Velocity2D LinearVelocity
    {
      get
      {
        InternalCalls.Rigidbody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
        return new Velocity2D(velocity);
      }
    }

    public void ApplyLinearImpulse(Vector2 impulse, Vector2 world_position, bool wake)
    {
      InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref world_position, wake);
    }

    public void ApplyLinearImpulse(Vector2 impulse, bool wake)
    {
      InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
    }
  }

  public class SpriteRenderer : Component
  {
    public Vector4 Color
    {
      get
      {
        InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
        return color;
      }
      set
      {
        InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
      }
    }
  }

  public class CircleRenderer : Component
  {
    public Vector4 Color
    {
      get
      {
        InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vector4 color);
        return color;
      }
      set
      {
        InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref value);
      }
    }

    public float Thickness
    {
      get
      {
        return InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
      }
      set
      {
        InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, ref value);
      }
    }

    public float Fade
    {
      get
      {
        return InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
      }
      set
      {
        InternalCalls.CircleRendererComponent_SetFade(Entity.ID, ref value);
      }
    }
  }

  public class Camera : Component
  {
    public bool Primary
    {
      get
      {
        return InternalCalls.CameraComponent_GetPrimaryStatus(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetPrimaryStatus(Entity.ID, ref value);
      }
    }

    public bool FixedAspectRatio
    {
      get
      {
        return InternalCalls.CameraComponent_GetFixedAspectRatio(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.ID, ref value);
      }
    }

    public float OrthographicSize
    {
      get
      {
        return InternalCalls.CameraComponent_GetOrthographicSize(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, ref value);
      }
    }

    public float OrthographicNearClip
    {
      get
      {
        return InternalCalls.CameraComponent_GetOrthographicNearClip(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetOrthographicNearClip(Entity.ID, ref value);
      }
    }

    public float OrthographicFarClip
    {
      get
      {
        return InternalCalls.CameraComponent_GetOrthographicFarClip(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetOrthographicFarClip(Entity.ID, ref value);
      }
    }

    public float PerspectiveFOV
    {
      get
      {
        return InternalCalls.CameraComponent_GetPerspectiveFOV(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetPerspectiveFOV(Entity.ID, ref value);
      }
    }

    public float PerspectiveNearClip
    {
      get
      {
        return InternalCalls.CameraComponent_GetPerspectiveNearClip(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetPerspectiveNearClip(Entity.ID, ref value);
      }
    }

    public float PerspectiveFarClip
    {
      get
      {
        return InternalCalls.CameraComponent_GetPerspectiveFarClip(Entity.ID);
      }
      set
      {
        InternalCalls.CameraComponent_SetPerspectiveFarClip(Entity.ID, ref value);
      }
    }

    public void SetOrthographic(float size, float near_clip, float far_clip)
    {
      InternalCalls.CameraComponent_SetOrthographic(Entity.ID, ref size, ref near_clip, ref far_clip);
    }

    public void SetPerspective(float fov, float near_clip, float far_clip)
    {
      InternalCalls.CameraComponent_SetPerspective(Entity.ID, ref fov, ref near_clip, ref far_clip);
    }
  }

  public class BoxCollider2D : Component
  {
    public Vector2 Offset
    {
      get
      {
        InternalCalls.BoxCollider2D_GetOffset(Entity.ID, out Vector2 offset);
        return offset;
      }
      set
      {
        InternalCalls.BoxCollider2D_SetOffset(Entity.ID, ref value);
      }
    }

    public Vector2 Size
    {
      get
      {
        InternalCalls.BoxCollider2D_GetSize(Entity.ID, out Vector2 size);
        return size;
      }
      set
      {
        InternalCalls.BoxCollider2D_SetSize(Entity.ID, ref value);
      }
    }

    public float Density
    {
      get
      {
        return InternalCalls.BoxCollider2D_GetDensity(Entity.ID);
      }
      set
      {
        InternalCalls.BoxCollider2D_SetDensity(Entity.ID, ref value);
      }
    }

    public float Friction
    {
      get
      {
        return InternalCalls.BoxCollider2D_GetFriction(Entity.ID);
      }
      set
      {
        InternalCalls.BoxCollider2D_SetFriction(Entity.ID, ref value);
      }
    }

    public float Restituition
    {
      get
      {
        return InternalCalls.BoxCollider2D_GetRestituition(Entity.ID);
      }
      set
      {
        InternalCalls.BoxCollider2D_SetRestituition(Entity.ID, ref value);
      }
    }

    public float RestituitionThreshold
    {
      get
      {
        return InternalCalls.BoxCollider2D_GetRestituitionThreshold(Entity.ID);
      }
      set
      {
        InternalCalls.BoxCollider2D_SetRestituitionThreshold(Entity.ID, ref value);
      }
    }
  }

  public class CircleCollider2D : Component
  {
    public Vector2 Offset
    {
      get
      {
        InternalCalls.CircleCollider2D_GetOffset(Entity.ID, out Vector2 offset);
        return offset;
      }
      set
      {
        InternalCalls.CircleCollider2D_SetOffset(Entity.ID, ref value);
      }
    }

    public float Radius
    {
      get
      {
        return InternalCalls.CircleCollider2D_GetRadius(Entity.ID);
      }
      set
      {
        InternalCalls.CircleCollider2D_SetRadius(Entity.ID, ref value);
      }
    }

    public float Density
    {
      get
      {
        return InternalCalls.CircleCollider2D_GetDensity(Entity.ID);
      }
      set
      {
        InternalCalls.CircleCollider2D_SetDensity(Entity.ID, ref value);
      }
    }

    public float Friction
    {
      get
      {
        return InternalCalls.CircleCollider2D_GetFriction(Entity.ID);
      }
      set
      {
        InternalCalls.CircleCollider2D_SetFriction(Entity.ID, ref value);
      }
    }

    public float Restituition
    {
      get
      {
        return InternalCalls.CircleCollider2D_GetRestituition(Entity.ID);
      }
      set
      {
        InternalCalls.CircleCollider2D_SetRestituition(Entity.ID, ref value);
      }
    }

    public float RestituitionThreshold
    {
      get
      {
        return InternalCalls.CircleCollider2D_GetRestituitionThreshold(Entity.ID);
      }
      set
      {
        InternalCalls.CircleCollider2D_SetRestituitionThreshold(Entity.ID, ref value);
      }
    }
  }
}
