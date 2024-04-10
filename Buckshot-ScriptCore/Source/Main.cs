using System;

namespace Buckshot {
  public class Main
  {
    public float Var { get; set; }

    public Main()
    {
      Console.WriteLine("Main Constructor");
    }

    public void PrintMessage()
    {
      Console.WriteLine("\n#2");
    }

    public void PrintMessageEx(string message)
    {
      Console.WriteLine(message);
    }

  }
}