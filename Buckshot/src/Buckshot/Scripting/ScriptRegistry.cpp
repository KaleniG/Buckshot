#include <bspch.h>
#include <glm/glm.hpp>
#include <mono/metadata/object.h>

#include "Buckshot/Core/Input.h"
#include "Buckshot/Scene/Scene.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scripting/ScriptRegistry.h"
#include "Buckshot/Scripting/ScriptEngine.h"

namespace Buckshot {

#define BS_ADD_INTERNAL_CALL(name) mono_add_internal_call("Buckshot.InternalCalls::" #name, name)

  // Input
  static bool Input_IsKeyDown(KeyCode keycode)
  {
    return Input::IsKeyPressed(keycode);
  }

  // Transform
  static void Entity_GetPosition(UUID entity_id, glm::vec3* out_position)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    Entity entity = scene->GetEntityByUUID(entity_id);

    *out_position = entity.GetComponent<TransformComponent>().Position;
  }

  static void Entity_SetPosition(UUID entity_id, glm::vec3* position)
  {
    Scene* scene = ScriptEngine::GetSceneContext();
    Entity entity = scene->GetEntityByUUID(entity_id);

    entity.GetComponent<TransformComponent>().Position = *position;
  }

  void ScriptRegistry::RegisterFunctions()
  {
    // Input
    BS_ADD_INTERNAL_CALL(Input_IsKeyDown);

    // Transform
    BS_ADD_INTERNAL_CALL(Entity_GetPosition);
    BS_ADD_INTERNAL_CALL(Entity_SetPosition);
  }

}