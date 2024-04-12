using System;
using Buckshot;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Sandbox
{
  public class Player : Entity
  {
    public void OnCreate()
    {
      Console.WriteLine("OnCreate");
      Console.WriteLine(ID);
    }

    public void OnUpdate(float timestep)
    {
      float speed = 1.5f;
      Vector3 velocity = Vector3.Zero;

      if (Input.IsKeyDown(KeyCode.A))
        velocity.x = -1.0f;
      if (Input.IsKeyDown(KeyCode.D))
        velocity.x = 1.0f;

      if (Input.IsKeyDown(KeyCode.W))
        velocity.y = 1.0f;
      if (Input.IsKeyDown(KeyCode.S))
        velocity.y = -1.0f;

      velocity *= speed;

      Vector3 pos = Position;
      pos += velocity * timestep;
      Position = pos;
    }
  }

}
