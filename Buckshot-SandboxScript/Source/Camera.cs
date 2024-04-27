using Buckshot;
using System;

namespace Sandbox
{
  public class Camera : Entity
  {
    public float DistanceFromPlayer = 35.0f;

    public void OnCreate()
    {
      Position = new Vector3(Position.xy, DistanceFromPlayer);
    }

    public void OnUpdate(float timestep)
    {
      Entity square = FindEntityByName("Square");
      if (square != null)
      {
        Position = new Vector3(square.Position.xy, DistanceFromPlayer);
      }

      float b = 233;

      Vector3 position = new Vector3(Position.xy, DistanceFromPlayer);
      GetComponent<TransformComponent>().Position = position;
    }
  }

}
