namespace Buckshot
{
  public struct Vector4
  {
    public float x;
    public float y;
    public float z;
    public float w;

    public static Vector4 zero = new Vector4(0.0f);

    public static Vector4 left = new Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
    public static Vector4 right = new Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    public static Vector4 up = new Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    public static Vector4 down = new Vector4(0.0f, -1.0f, 0.0f, 0.0f);
    public static Vector4 front = new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    public static Vector4 behind = new Vector4(0.0f, 0.0f, -1.0f, 0.0f);

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

    public Vector3 xyz
    {
      get
      {
        return new Vector3(x, y, z);
      }
      set
      {
        x = value.x;
        y = value.y;
        z = value.z;
      }
    }

    public Vector4(float scalar)
    {
      this.x = scalar;
      this.y = scalar;
      this.z = scalar;
      this.w = scalar;
    }

    public Vector4(float x, float y, float z, float w)
    {
      this.x = x;
      this.y = y;
      this.z = z;
      this.w = w;
    }

    public static Vector4 operator *(Vector4 vector, float scalar)
    {
      return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    }

    public static Vector4 operator /(Vector4 vector, float scalar)
    {
      return new Vector4(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar);
    }

    public static Vector4 operator +(Vector4 vector, float scalar)
    {
      return new Vector4(vector.x + scalar, vector.y + scalar, vector.z + scalar, vector.w + scalar);
    }

    public static Vector4 operator -(Vector4 vector, float scalar)
    {
      return new Vector4(vector.x - scalar, vector.y - scalar, vector.z - scalar, vector.w - scalar);
    }

    public static Vector4 operator +(Vector4 vector_a, Vector4 vector_b)
    {
      return new Vector4(vector_a.x + vector_b.x, vector_a.y + vector_b.y, vector_a.z + vector_b.z, vector_a.w + vector_b.w);
    }

    public static Vector4 operator -(Vector4 vector_a, Vector4 vector_b)
    {
      return new Vector4(vector_a.x - vector_b.x, vector_a.y - vector_b.y, vector_a.z - vector_b.z, vector_a.w - vector_b.w);
    }

    public static Vector4 operator *(Vector4 vector_a, Vector4 vector_b)
    {
      return new Vector4(vector_a.x * vector_b.x, vector_a.y * vector_b.y, vector_a.z * vector_b.z, vector_a.w * vector_b.w);
    }

    public static Vector4 operator /(Vector4 vector_a, Vector4 vector_b)
    {
      return new Vector4(vector_a.x / vector_b.x, vector_a.y / vector_b.y, vector_a.z / vector_b.z, vector_a.w / vector_b.w);
    }
  }
}
