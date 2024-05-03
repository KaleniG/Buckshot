using Buckshot;
using System;

namespace Sandbox
{
  public class CameraEntity : Entity
  {
    private Transform m_Transform;
    
    public float DistanceFromPlayer = 35.0f;

    public void OnCreate()
    {
      
      m_Transform = this.GetComponent<Transform>();
      m_Transform.Position = new Vector3(m_Transform.Position.xy, DistanceFromPlayer);
    }

    public void OnUpdate(float timestep)
    {
      Entity square = FindEntityByName("Square");
      if (square != null)
      {
        Transform square_transform = square.GetComponent<Transform>();
        m_Transform.Position = new Vector3(square_transform.Position.xy, DistanceFromPlayer);
      }

      Vector3 position = new Vector3(m_Transform.Position.xy, DistanceFromPlayer);
      m_Transform.Position = position;
    }
  }

}
