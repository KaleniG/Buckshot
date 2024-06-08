#include <bspch.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Physics/Physics2D.h"
#include "Buckshot/Scene/Scene.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scripting/ScriptRegistry.h"
#include "Buckshot/Scripting/ScriptEngine.h"

namespace Buckshot {

  #define BS_ADD_INTERNAL_CALL(name) mono_add_internal_call("Buckshot.InternalCalls::" #name, name)

  static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

  // Input
  static bool Input_IsKeyPressed(KeyCode keycode)
  {
    return Input::IsKeyPressed(keycode);
  }
  static bool Input_IsMouseButtonPressed(MouseCode mousecode)
  {
    return Input::IsMouseButtonPressed(mousecode);
  }
  static void Input_GetMousePosition(glm::vec2* out_position)
  {
    *out_position = Input::GetMousePos();
  }

  //Entity
  static bool Entity_HasComponent(UUID entity_id, MonoReflectionType* component_type)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    MonoType* mono_component_type = mono_reflection_type_get_type(component_type);
    BS_ASSERT(s_EntityHasComponentFunctions.find(mono_component_type) != s_EntityHasComponentFunctions.end(), "MonoType not found in the conversion registry");
    return s_EntityHasComponentFunctions.at(mono_component_type)(entity);
  }
  static uint64_t Entity_FindEntityByName(MonoString* name)
  {
    char* entity_name = mono_string_to_utf8(name);

    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByName(entity_name);

    mono_free(entity_name);

    if (entity)
      return entity.GetUUID();
    else
      return 0;
  }
  static MonoString* Entity_GetName(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return mono_string_new_wrapper(entity.GetName().c_str());
  }

  // ScriptEngine
  static MonoObject* ScriptEngine_GetScriptInstance(UUID entity_id)
  {
    return ScriptEngine::GetEntityMonoScriptInstance(entity_id);
  }

  // TransformComponent
  static void TransformComponent_GetPosition(UUID entity_id, glm::vec3* out_position)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_position = entity.GetComponent<TransformComponent>().Position;
  }
  static void TransformComponent_SetPosition(UUID entity_id, glm::vec3* position)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<TransformComponent>().Position = *position;
  }
  static void TransformComponent_GetRotation(UUID entity_id, glm::vec3* out_rotation)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_rotation = entity.GetComponent<TransformComponent>().Rotation;
  }
  static void TransformComponent_SetRotation(UUID entity_id, glm::vec3* rotation)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<TransformComponent>().Rotation = *rotation;
  }
  static void TransformComponent_GetScale(UUID entity_id, glm::vec3* out_scale)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_scale = entity.GetComponent<TransformComponent>().Scale;
  }
  static void TransformComponent_SetScale(UUID entity_id, glm::vec3* scale)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<TransformComponent>().Scale = *scale;
  }

  // Rigidbody2DComponent
  static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entity_id, glm::vec2* impulse, glm::vec2* world_position, bool wake)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RuntimeBody;
    body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(world_position->x, world_position->y), wake);
  }
  static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entity_id, glm::vec2* impulse, bool wake)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RuntimeBody;
    body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
  }
  static bool Rigidbody2DComponent_GetFixedRotation(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<Rigidbody2DComponent>().FixedRotation;
  }
  static void Rigidbody2DComponent_SetFixedRotation(UUID entity_id, bool* status)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<Rigidbody2DComponent>().FixedRotation = *status;
  }
  static void Rigidbody2DComponent_GetLinearVelocity(UUID entity_id, glm::vec2* velocity)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RuntimeBody;
    *velocity = glm::vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y);
  }
  static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetBodyType(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RuntimeBody;
    return Utilities::Rigidbody2D_Box2DToBuckshot(body->GetType());
  }
  static void Rigidbody2DComponent_SetBodyType(UUID entity_id, Rigidbody2DComponent::BodyType type)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
    b2Body* body = (b2Body*)rb2d.RuntimeBody;
    body->SetType(Utilities::Rigidbody2D_BuckshotToBox2D(type));
    rb2d.Type = type;
  }

  // SpriteRendererComponent
  static void SpriteRendererComponent_GetColor(UUID entity_id, glm::vec4* out_color)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_color = entity.GetComponent<SpriteRendererComponent>().Color;
  }
  static void SpriteRendererComponent_SetColor(UUID entity_id, glm::vec4* color)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<SpriteRendererComponent>().Color = *color;
  }

  // CircleRendererComponent
  static void CircleRendererComponent_GetColor(UUID entity_id, glm::vec4* out_color)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_color = entity.GetComponent<CircleRendererComponent>().Color;
  }
  static void CircleRendererComponent_SetColor(UUID entity_id, glm::vec4* color)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleRendererComponent>().Color = *color;
  }
  static float CircleRendererComponent_GetThickness(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleRendererComponent>().Thickness;
  }
  static void CircleRendererComponent_SetThickness(UUID entity_id, float thickness)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
  }
  static float CircleRendererComponent_GetFade(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleRendererComponent>().Fade;
  }
  static void CircleRendererComponent_SetFade(UUID entity_id, float fade)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleRendererComponent>().Fade = fade;
  }

  // CameraComponent
  static bool CameraComponent_GetPrimaryStatus(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Primary;
  }
  static void CameraComponent_SetPrimaryStatus(UUID entity_id, bool* status)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Primary = *status;
  }

  static bool CameraComponent_GetFixedAspectRatio(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().FixedAspectRatio;
  }
  static void CameraComponent_SetFixedAspectRatio(UUID entity_id, bool* status)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().FixedAspectRatio = *status;
  }

  static float CameraComponent_GetOrthographicSize(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
  }
  static float CameraComponent_GetOrthographicNearClip(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetOrthographicNearClip();
  }
  static float CameraComponent_GetOrthographicFarClip(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetOrthographicFarClip();
  }
  static void CameraComponent_SetOrthographic(UUID entity_id, float size, float near_clip, float far_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetOrthographic(size, near_clip, far_clip);
  }
  static void CameraComponent_SetOrthographicSize(UUID entity_id, float size)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(size);
  }
  static void CameraComponent_SetOrthographicNearClip(UUID entity_id, float near_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetOrthographicNearClip(near_clip);
  }
  static void CameraComponent_SetOrthographicFarClip(UUID entity_id, float far_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetOrthographicFarClip(far_clip);
  }

  static float CameraComponent_GetPerspectiveFOV(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveFOV();
  }
  static float CameraComponent_GetPerspectiveNearClip(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveNearClip();
  }
  static float CameraComponent_GetPerspectiveFarClip(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveFarClip();
  }
  static void CameraComponent_SetPerspective(UUID entity_id, float fov, float near_clip, float far_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetPerspective(fov, near_clip, far_clip);
  }
  static void CameraComponent_SetPerspectiveFOV(UUID entity_id, float fov)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetPerspectiveFOV(fov);
  }
  static void CameraComponent_SetPerspectiveNearClip(UUID entity_id, float near_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetPerspectiveNearClip(near_clip);
  }
  static void CameraComponent_SetPerspectiveFarClip(UUID entity_id, float far_clip)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CameraComponent>().Camera.SetPerspectiveFarClip(far_clip);
  }

  // BoxCollider2D
  static void BoxCollider2D_GetOffset(UUID entity_id, glm::vec2* out_offset)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_offset = entity.GetComponent<BoxCollider2DComponent>().Offset;
  }
  static void BoxCollider2D_SetOffset(UUID entity_id, glm::vec2* offset)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().Offset = *offset;
  }
  static void BoxCollider2D_GetSize(UUID entity_id, glm::vec2* out_size)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_size = entity.GetComponent<BoxCollider2DComponent>().Size;
  }
  static void BoxCollider2D_SetSize(UUID entity_id, glm::vec2* size)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().Size = *size;
  }
  static float BoxCollider2D_GetDensity(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<BoxCollider2DComponent>().Density;
  }
  static void BoxCollider2D_SetDensity(UUID entity_id, float density)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().Density = density;
  }
  static float BoxCollider2D_GetFriction(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<BoxCollider2DComponent>().Friction;
  }
  static void BoxCollider2D_SetFriction(UUID entity_id, float friction)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().Friction = friction;
  }
  static float BoxCollider2D_GetRestituition(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<BoxCollider2DComponent>().Restituition;
  }
  static void BoxCollider2D_SetRestituition(UUID entity_id, float restituition)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().Restituition = restituition;
  }
  static float BoxCollider2D_GetRestituitionThreshold(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<BoxCollider2DComponent>().RestituitionThreshold;
  }
  static void BoxCollider2D_SetRestituitionThreshold(UUID entity_id, float restituition_threshold)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<BoxCollider2DComponent>().RestituitionThreshold = restituition_threshold;
  }

  // CircleCollider2D
  static void CircleCollider2D_GetOffset(UUID entity_id, glm::vec2* out_offset)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    *out_offset = entity.GetComponent<CircleCollider2DComponent>().Offset;
  }
  static void CircleCollider2D_SetOffset(UUID entity_id, glm::vec2* offset)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().Offset = *offset;
  }
  static float CircleCollider2D_GetRadius(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleCollider2DComponent>().Radius;
  }
  static void CircleCollider2D_SetRadius(UUID entity_id, float radius)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().Radius = radius;
  }
  static float CircleCollider2D_GetDensity(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleCollider2DComponent>().Density;
  }
  static void CircleCollider2D_SetDensity(UUID entity_id, float density)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().Density = density;
  }
  static float CircleCollider2D_GetFriction(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleCollider2DComponent>().Friction;
  }
  static void CircleCollider2D_SetFriction(UUID entity_id, float friction)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().Friction = friction;
  }
  static float CircleCollider2D_GetRestituition(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleCollider2DComponent>().Restituition;
  }
  static void CircleCollider2D_SetRestituition(UUID entity_id, float restituition)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().Restituition = restituition;
  }
  static float CircleCollider2D_GetRestituitionThreshold(UUID entity_id)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    return entity.GetComponent<CircleCollider2DComponent>().RestituitionThreshold;
  }
  static void CircleCollider2D_SetRestituitionThreshold(UUID entity_id, float restituition_threshold)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    BS_ASSERT(scene, "No scene");
    Entity entity = scene->GetEntityByUUID(entity_id);
    BS_ASSERT(entity, "No entity");

    entity.GetComponent<CircleCollider2DComponent>().RestituitionThreshold = restituition_threshold;
  }

  template<typename... Component>
  static void RegisterComponent(ComponentGroup<Component...>)
  {
    ([]()
    {
      std::string_view type_name = typeid(Component).name();
      size_t pos = type_name.find_last_of(':');
      std::string_view component_name = type_name.substr(pos + 1);
      component_name.remove_suffix(9); // Removes "Component" from the type's name
      std::string managed_type_name = fmt::format("Buckshot.{0}", component_name);

      MonoType* managed_type = mono_reflection_type_from_name(managed_type_name.data(), ScriptEngine::GetCoreAssemblyImage());
      if (!managed_type)
      {
        BS_ERROR("Failed to detect the managed type \"{0}\"", managed_type_name);
        return;
      }
      s_EntityHasComponentFunctions[managed_type] = [](Entity entity) { return entity.HasComponent<Component>(); };
    }(), ...);
  }

  template<typename... Component>
  static void RegisterComponent()
  {
    s_EntityHasComponentFunctions.clear();
    RegisterComponent<Component...>();
  }

  void ScriptRegistry::RegisterComponents()
  {
    RegisterComponent(AllComponents{});
  }

  void ScriptRegistry::RegisterFunctions()
  {
    // Input
    BS_ADD_INTERNAL_CALL(Input_IsKeyPressed);
    BS_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
    BS_ADD_INTERNAL_CALL(Input_GetMousePosition);

    // Entity
    BS_ADD_INTERNAL_CALL(Entity_HasComponent);
    BS_ADD_INTERNAL_CALL(Entity_FindEntityByName);
    BS_ADD_INTERNAL_CALL(Entity_GetName);

    // ScriptEngine
    BS_ADD_INTERNAL_CALL(ScriptEngine_GetScriptInstance);

    // Transform
    BS_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
    BS_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
    BS_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
    BS_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
    BS_ADD_INTERNAL_CALL(TransformComponent_GetScale);
    BS_ADD_INTERNAL_CALL(TransformComponent_SetScale);

    // Rigidbody2D
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetFixedRotation);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetFixedRotation);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetBodyType);
    BS_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetBodyType);

    // SpriteRenderer
    BS_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
    BS_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);

    // CircleRenderer
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
    BS_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

    // Camera
    BS_ADD_INTERNAL_CALL(CameraComponent_GetPrimaryStatus);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetPrimaryStatus);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetFixedAspectRatio);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetFixedAspectRatio);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetOrthographic);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicSize);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicSize);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicNearClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicNearClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicFarClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicFarClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetPerspective);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFOV);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFOV);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveNearClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveNearClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFarClip);
    BS_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFarClip);

    // BoxCollider2D
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetOffset);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetOffset);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetSize);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetSize);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetDensity);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetDensity);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetFriction);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetFriction);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetRestituition);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetRestituition);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_GetRestituitionThreshold);
    BS_ADD_INTERNAL_CALL(BoxCollider2D_SetRestituitionThreshold);

    // CircleCollider2D
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetOffset);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetOffset);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetRadius);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetRadius);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetDensity);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetDensity);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetFriction);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetFriction);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetRestituition);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetRestituition);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_GetRestituitionThreshold);
    BS_ADD_INTERNAL_CALL(CircleCollider2D_SetRestituitionThreshold);
  }

}