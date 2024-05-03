namespace Buckshot
{
  public class Input
  {
    public static bool IsKeyPressed(KeyCode keycode)
    {
      return InternalCalls.Input_IsKeyPressed(keycode);
    }

    public static bool IsMouseButtonPressed(MouseCode mousecode)
    {
      return InternalCalls.Input_IsMouseButtonPressed(mousecode);
    }

    public static Vector2 GetMousePosition()
    {
      // DO NOT USE!!!
      InternalCalls.Input_GetMousePosition(out Vector2 position);
      return position;
    }
  }
}
