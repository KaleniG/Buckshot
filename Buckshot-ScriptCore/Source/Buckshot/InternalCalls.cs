using System;
using System.Runtime.CompilerServices;

namespace Buckshot
{
  public static class InternalCalls
  {
    // Input
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Input_IsKeyPressed(KeyCode keycode);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Input_IsMouseButtonPressed(MouseCode mousecode);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Input_GetMousePosition(out Vector2 position);

    // Entity
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Entity_HasComponent(ulong entity_id, Type component_type);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ulong Entity_FindEntityByName(string name);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static string Entity_GetName(ulong entity_id);

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

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Rigidbody2DComponent_GetFixedRotation(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Rigidbody2DComponent_SetFixedRotation(ulong entity_id, ref bool status);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Rigidbody2DComponent_GetLinearVelocity(ulong entity_id, out Vector2 velocity);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static Rigidbody2D.BodyType2D Rigidbody2DComponent_GetBodyType(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Rigidbody2DComponent_SetBodyType(ulong entity_id, ref Rigidbody2D.BodyType2D type);

    // SpriteRenderer
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void SpriteRendererComponent_GetColor(ulong entity_id, out Vector4 color);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void SpriteRendererComponent_SetColor(ulong entity_id, ref Vector4 color);

    // CircleRenderer
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleRendererComponent_GetColor(ulong entity_id, out Vector4 color);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleRendererComponent_SetColor(ulong entity_id, ref Vector4 color);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleRendererComponent_GetThickness(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleRendererComponent_SetThickness(ulong entity_id, ref float thickness);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleRendererComponent_GetFade(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleRendererComponent_SetFade(ulong entity_id, ref float fade);

    // Camera
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool CameraComponent_GetPrimaryStatus(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetPrimaryStatus(ulong entity_id, ref bool status);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool CameraComponent_GetFixedAspectRatio(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetFixedAspectRatio(ulong entity_id, ref bool status);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetOrthographic(ulong entity_id, ref float size, ref float near_clip, ref float far_clip);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetOrthographicSize(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetOrthographicNearClip(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetOrthographicFarClip(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetOrthographicSize(ulong entity_id, ref float size);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetOrthographicNearClip(ulong entity_id, ref float near_clip);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetOrthographicFarClip(ulong entity_id, ref float far_clip);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetPerspective(ulong entity_id, ref float fov, ref float near_clip, ref float far_clip);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetPerspectiveFOV(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetPerspectiveNearClip(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CameraComponent_GetPerspectiveFarClip(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetPerspectiveFOV(ulong entity_id, ref float fov);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetPerspectiveNearClip(ulong entity_id, ref float near_clip);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CameraComponent_SetPerspectiveFarClip(ulong entity_id, ref float far_clip);

    // BoxCollider2D
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_GetOffset(ulong entity_id, out Vector2 offset);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetOffset(ulong entity_id, ref Vector2 offset);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_GetSize(ulong entity_id, out Vector2 size);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetSize(ulong entity_id, ref Vector2 size);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float BoxCollider2D_GetDensity(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetDensity(ulong entity_id, ref float density);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float BoxCollider2D_GetFriction(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetFriction(ulong entity_id, ref float friction);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float BoxCollider2D_GetRestituition(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetRestituition(ulong entity_id, ref float restituition);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float BoxCollider2D_GetRestituitionThreshold(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void BoxCollider2D_SetRestituitionThreshold(ulong entity_id, ref float restituition_threshold);

    // CircleCollider2D
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_GetOffset(ulong entity_id, out Vector2 offset);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetOffset(ulong entity_id, ref Vector2 offset);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleCollider2D_GetRadius(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetRadius(ulong entity_id, ref float radius);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleCollider2D_GetDensity(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetDensity(ulong entity_id, ref float density);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleCollider2D_GetFriction(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetFriction(ulong entity_id, ref float friction);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleCollider2D_GetRestituition(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetRestituition(ulong entity_id, ref float restituition);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float CircleCollider2D_GetRestituitionThreshold(ulong entity_id);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void CircleCollider2D_SetRestituitionThreshold(ulong entity_id, ref float restituition_threshold);
  }
}
