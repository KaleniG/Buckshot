namespace Buckshot
{
  public struct Vector2
  {
    public float x;
    public float y;

    public static Vector2 zero = new Vector2(0.0f);

    public static Vector2 left = new Vector2(-1.0f, 0.0f);
    public static Vector2 right = new Vector2(1.0f, 0.0f);
    public static Vector2 up = new Vector2(0.0f, 1.0f);
    public static Vector2 down = new Vector2(0.0f, -1.0f);

    public Vector2(float scalar)
    {
      this.x = scalar;
      this.y = scalar;
    }

    public Vector2(float x, float y)
    {
      this.x = x;
      this.y = y;
    }

    public static Vector2 operator *(Vector2 vector, float scalar)
    {
      return new Vector2(vector.x * scalar, vector.y * scalar);
    }

    public static Vector2 operator /(Vector2 vector, float scalar)
    {
      return new Vector2(vector.x / scalar, vector.y / scalar);
    }

    public static Vector2 operator +(Vector2 vector, float scalar)
    {
      return new Vector2(vector.x + scalar, vector.y + scalar);
    }

    public static Vector2 operator -(Vector2 vector, float scalar)
    {
      return new Vector2(vector.x - scalar, vector.y - scalar);
    }

    public static Vector2 operator +(Vector2 vector_a, Vector2 vector_b)
    {
      return new Vector2(vector_a.x + vector_b.x, vector_a.y + vector_b.y);
    }

    public static Vector2 operator -(Vector2 vector_a, Vector2 vector_b)
    {
      return new Vector2(vector_a.x - vector_b.x, vector_a.y - vector_b.y);
    }

    public static Vector2 operator *(Vector2 vector_a, Vector2 vector_b)
    {
      return new Vector2(vector_a.x * vector_b.x, vector_a.y * vector_b.y);
    }

    public static Vector2 operator /(Vector2 vector_a, Vector2 vector_b)
    {
      return new Vector2(vector_a.x / vector_b.x, vector_a.y / vector_b.y);
    }
  }
}
