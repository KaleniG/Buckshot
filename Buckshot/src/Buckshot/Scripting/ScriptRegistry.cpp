#include <bspch.h>
#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/object.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Scene/Scene.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scripting/ScriptRegistry.h"
#include "Buckshot/Scripting/ScriptEngine.h"

namespace Buckshot {

  #define BS_ADD_INTERNAL_CALL(name) mono_add_internal_call("Buckshot.InternalCalls::" #name, name)

  static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

  // Input
  static bool Input_IsKeyDown(KeyCode keycode)
  {
    return Input::IsKeyPressed(keycode);
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

  template<typename... Component>
  static void RegisterComponent(ComponentGroup<Component...>)
  {
    ([]()
    {
      std::string_view type_name = typeid(Component).name();
      size_t pos = type_name.find_last_of(':');
      std::string_view component_name = type_name.substr(pos + 1);
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
    BS_ADD_INTERNAL_CALL(Input_IsKeyDown);

    // Entity
    BS_ADD_INTERNAL_CALL(Entity_HasComponent);
    BS_ADD_INTERNAL_CALL(Entity_FindEntityByName);

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
  }

}