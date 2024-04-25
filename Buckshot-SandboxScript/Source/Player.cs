using Buckshot;
using System;

namespace Sandbox
{
  public class Player : Entity
  {
    private TransformComponent m_Transform;
    private Rigidbody2DComponent m_Rigidbody2D;

    public float Speed;

    public void OnCreate()
    {
      m_Transform = GetComponent<TransformComponent>();
      m_Rigidbody2D = GetComponent<Rigidbody2DComponent>();
    }

    public void OnUpdate(float timestep)
    {
      Vector3 velocity = Vector3.zero;

      if (Input.IsKeyDown(KeyCode.A))
        velocity.x = -1.0f;
      if (Input.IsKeyDown(KeyCode.D))
        velocity.x = 1.0f;

      if (Input.IsKeyDown(KeyCode.W))
        velocity.y = 1.0f;
      if (Input.IsKeyDown(KeyCode.S))
        velocity.y = -1.0f;

      velocity *= 0.5f * Speed;
      m_Rigidbody2D.ApplyLinearImpulse(velocity.xy, true);
    }
  }

}
