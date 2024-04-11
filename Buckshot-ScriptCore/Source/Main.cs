using System;
using System.Runtime.CompilerServices;

namespace Buckshot {

  public struct Vector3
  {
    public float x;
    public float y;
    public float z;

    public Vector3(float x, float y, float z)
    {
      this.x = x;
      this.y = y;
      this.z = z;
    }
  }

  public static class InternalCalls
  {
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void NativeLog(string text, int parameter);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void NativeLogVector(ref Vector3 vector, out Vector3 result);
  }

  public class Entity
  {
    public void PrintMessage()
    {
      Console.WriteLine("Message");
    }
  }
}