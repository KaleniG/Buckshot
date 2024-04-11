#include <bspch.h>
#include <mono/metadata/object.h>

#include "Buckshot/Scripting/ScriptRegistry.h"

namespace Buckshot {

#define BS_ADD_INTERNAL_CALL(name) mono_add_internal_call("Buckshot.InternalCalls::" #name, name)

  void ScriptRegistry::RegisterFunctions()
  {

  }

}