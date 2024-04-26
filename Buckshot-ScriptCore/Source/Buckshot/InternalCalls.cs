using System;
using System.Runtime.CompilerServices;

namespace Buckshot
{
  public static class InternalCalls
  {
    // Input
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Input_IsKeyDown(KeyCode keycode);

    // Entity
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Entity_HasComponent(ulong entity_id, Type component_type);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ulong Entity_FindEntityByName(string name);

    // ScriptEngine
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static object ScriptEngine_GetScriptInstance(ulong entity_id);

    // Transform
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_GetPosition(ulong entity_id, out Vector3 position);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_SetPosition(ulong entity_id, ref Vector3 position);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_GetRotation(ulong entity_id, out Vector3 rotation);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_SetRotation(ulong entity_id, ref Vector3 rotation);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_GetScale(ulong entity_id, out Vector3 scale);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_SetScale(ulong entity_id, ref Vector3 scale);

    // Rigidbody2D
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Rigidbody2DComponent_ApplyLinearImpulse(ulong entity_id, ref Vector2 impulse, ref Vector2 world_position, bool wake);
    
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong entity_id, ref Vector2 impulse, bool wake);


  }
}
