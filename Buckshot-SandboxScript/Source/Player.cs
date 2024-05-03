using Buckshot;
using System;

namespace Sandbox
{
  public class Player : Entity
  {
    private Rigidbody2D m_Rigidbody2D;
    private CircleCollider2D m_CircleCollider2D;
    private Transform m_Transform;
    public float Speed ;

    public void OnCreate()
    {
      m_CircleCollider2D = this.GetComponent<CircleCollider2D>();
      m_Rigidbody2D = this.GetComponent<Rigidbody2D>();
      m_Transform = this.GetComponent<Transform>();
    }

    public void OnUpdate(float timestep)
    {
      Vector3 velocity = Vector3.zero;

      if (Input.IsKeyPressed(KeyCode.A))
        velocity.x = -1.0f;
      if (Input.IsKeyPressed(KeyCode.D))
        velocity.x = 1.0f;

      if (Input.IsKeyPressed(KeyCode.W))
        velocity.y = 1.0f;
      if (Input.IsKeyPressed(KeyCode.S))
        velocity.y = -1.0f;

      if (Input.IsKeyPressed(KeyCode.Space))
      {
      }

      velocity *= Speed;
      m_Rigidbody2D.ApplyLinearImpulse(velocity.xy, true);
    }
  }

}
