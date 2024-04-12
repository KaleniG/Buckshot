#pragma once

#include "Buckshot/Scene/Scene.h"

extern "C" 
{
  typedef struct _MonoAssembly MonoAssembly;
  typedef struct _MonoObject MonoObject;
  typedef struct _MonoMethod MonoMethod;
  typedef struct _MonoClass MonoClass;
  typedef struct _MonoImage MonoImage;
}

namespace Buckshot {

  class ScriptClass
  {
  public:
    ScriptClass() = default;
    ScriptClass(const std::string& class_namespace, const std::string& class_name);

    MonoObject* Instantiate();
    MonoMethod* GetMethod(const std::string& name, int parameter_count);
    MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** parameters = nullptr);

  private:
    std::string m_ClassNamespace;
    std::string m_ClassName;
    MonoClass* m_Class = nullptr;
  };

  class ScriptInstance
  {
  public:
    ScriptInstance(Ref<ScriptClass> script_class, Entity entity);

    void InvokeOnCreate();
    void InvokeOnUpdate(Timestep timestep);
  private:
    Ref<ScriptClass> m_ScriptClass;

    MonoObject* m_Instance = nullptr;
    MonoMethod* m_Constructor = nullptr;
    MonoMethod* m_OnCreateMethod = nullptr;
    MonoMethod* m_OnUpdateMethod = nullptr;
  };

  class ScriptEngine
  {
  public:
    static void Init();
    static void Shutdown();

    static void OnRuntimeStart(Scene* scene);
    static void OnRuntimeStop();

    static bool EntityClassExists(const std::string& full_name);
    static void OnCreateEntity(Entity& entity);
    static void OnUpdateEntity(Entity& entity, Timestep timestep);

    static void LoadAssembly(const std::filesystem::path& filepath);
    static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
    static Scene* GetSceneContext();

  private:
    static void InitMono();
    static void ShutdownMono();

    static MonoObject* InstantiateClass(MonoClass* mono_class);
    static void LoadAssemblyClasses(MonoAssembly* assembly);

    friend class ScriptClass;
  };

}