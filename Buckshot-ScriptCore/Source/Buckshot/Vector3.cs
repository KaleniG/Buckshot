namespace Buckshot
{
  public struct Vector3
  {
    public float x;
    public float y;
    public float z;

    public static Vector3 zero = new Vector3(0.0f);

    public static Vector3 left = new Vector3(-1.0f, 0.0f, 0.0f);
    public static Vector3 right = new Vector3(1.0f, 0.0f, 0.0f);
    public static Vector3 up = new Vector3(0.0f, 1.0f, 0.0f);
    public static Vector3 down = new Vector3(0.0f, -1.0f, 0.0f);
    public static Vector3 front = new Vector3(0.0f, 0.0f, 1.0f);
    public static Vector3 behind = new Vector3(0.0f, 0.0f, -1.0f);

    public Vector2 xy
    {
      get
      {
        return new Vector2(x, y);
      }
      set
      {
        x = value.x;
        y = value.y;
      }
    }

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

    public Vector3(Vector2 vec2, float z)
    {
      this.x = vec2.x;
      this.y = vec2.y;
      this.z = z;
    }

    public Vector3(float x, Vector2 vec2)
    {
      this.x = x;
      this.y = vec2.y;
      this.z = vec2.x;
    }

    public static Vector3 operator *(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    public static Vector3 operator /(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
    }

    public static Vector3 operator +(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x + scalar, vector.y + scalar, vector.z + scalar);
    }

    public static Vector3 operator -(Vector3 vector, float scalar)
    {
      return new Vector3(vector.x - scalar, vector.y - scalar, vector.z - scalar);
    }

    public static Vector3 operator +(Vector3 vector_a, Vector3 vector_b)
    {
      return new Vector3(vector_a.x + vector_b.x, vector_a.y + vector_b.y, vector_a.z + vector_b.z);
    }

    public static Vector3 operator -(Vector3 vector_a, Vector3 vector_b)
    {
      return new Vector3(vector_a.x - vector_b.x, vector_a.y - vector_b.y, vector_a.z - vector_b.z);
    }

    public static Vector3 operator *(Vector3 vector_a, Vector3 vector_b)
    {
      return new Vector3(vector_a.x * vector_b.x, vector_a.y * vector_b.y, vector_a.z * vector_b.z);
    }

    public static Vector3 operator /(Vector3 vector_a, Vector3 vector_b)
    {
      return new Vector3(vector_a.x / vector_b.x, vector_a.y / vector_b.y, vector_a.z / vector_b.z);
    }
  }
}