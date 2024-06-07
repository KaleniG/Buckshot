using System;

namespace Buckshot
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
}
