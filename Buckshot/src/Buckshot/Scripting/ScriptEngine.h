#pragma once

#include <map>

#include "Buckshot/Scene/Scene.h"

extern "C" 
{
  typedef struct _MonoClassField MonoClassField;
  typedef struct _MonoAssembly MonoAssembly;
  typedef struct _MonoObject MonoObject;
  typedef struct _MonoMethod MonoMethod;
  typedef struct _MonoClass MonoClass;
  typedef struct _MonoImage MonoImage;
}

namespace Buckshot {

  enum class ScriptFieldType
  {
    None,
    Bool,
    Float, Double, Decimal,
    Long, Int, Short, Byte, 
    ULong, UInt, UShort, UByte,
    Vector2, Vector3, Vector4,
    Entity
  };

  struct ScriptField
  {
    std::string Name;
    ScriptFieldType Type = ScriptFieldType::None;
    MonoClassField* MonoField = nullptr;

    ScriptField() = default;
    ScriptField(std::string name, ScriptFieldType type, MonoClassField* mono_class_field) 
      : Name(name), Type(type), MonoField(mono_class_field) {}
  };

  struct ScriptFieldInstance
  {
    ScriptField Field;

    ScriptFieldInstance()
    {
      std::memset(m_DataBuffer, 0, sizeof(m_DataBuffer));
    }

    template<typename T>
    T GetValue()
    {
      static_assert(sizeof(T) <= 16, "Type too large");
      return *(T*)m_DataBuffer;
    }

    template<typename T>
    void SetValue(T value)
    {
      static_assert(sizeof(T) <= 16, "Type too large");
      std::memcpy(m_DataBuffer, &value, sizeof(T));
    }

  private:
    uint8_t m_DataBuffer[16];

    friend class ScriptEngine;
  };

  using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

  class ScriptClass
  {
  public:
    ScriptClass() = default;
    ScriptClass(const std::string& class_namespace, const std::string& class_name, bool is_core = false);
    MonoObject* Instantiate();
    MonoMethod* GetMethod(const std::string& name, int parameter_count);
    MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** parameters = nullptr);

    const std::map<std::string, ScriptField>& GetScriptFields() const { return m_Fields; }

  private:
    std::string m_ClassNamespace;
    std::string m_ClassName;
    std::map<std::string, ScriptField> m_Fields;
    MonoClass* m_Class = nullptr;

    friend class ScriptEngine;
  };

  class ScriptInstance
  {
  public:
    ScriptInstance(Ref<ScriptClass> script_class, Entity entity);
    void InvokeOnCreate();
    void InvokeOnUpdate(Timestep timestep);
    Ref<ScriptClass> GetScriptClass();
    MonoObject* GetScriptInstance() { return m_Instance; }

    template<typename T>
    T GetFieldValue(const std::string& name)
    {
      bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
      if (!success)
      {
        BS_ERROR("Unsuccessful getting of field value \"{0}\"", name);
        return T();
      }
      return *(T*)s_FieldValueBuffer;
    }

    template<typename T>
    void SetFieldValue(const std::string& name, const T& value)
    {
      bool success = SetFieldValueInternal(name, &value);
      if (!success)
      {
        BS_ERROR("Unsuccessful setting of field value \"{0}\"", name);
      }
    }

  private:
    bool GetFieldValueInternal(const std::string& name, void* buffer);
    bool SetFieldValueInternal(const std::string& name, const void* value);

  private:
    Ref<ScriptClass> m_ScriptClass;
    MonoObject* m_Instance = nullptr;
    MonoMethod* m_Constructor = nullptr;
    MonoMethod* m_OnCreateMethod = nullptr;
    MonoMethod* m_OnUpdateMethod = nullptr;

    inline static char s_FieldValueBuffer[16];

    friend class ScriptEngine;
  };

  class ScriptEngine
  {
  public:
    static void Init();
    static void Shutdown();
    static void OnRuntimeStop();
    static void OnRuntimeStart(Scene* scene);
    static void OnCreateEntity(Entity& entity);
    static void OnUpdateEntity(Entity& entity, Timestep timestep);
    static void LoadAssembly(const std::filesystem::path& filepath);
    static void LoadAppAssembly(const std::filesystem::path& filepath);
    static void ReloadAssenbly();
    
    static bool EntityClassExists(const std::string& full_name);

    static Scene* GetSceneContext();
    static MonoImage* GetCoreAssemblyImage();
    static ScriptFieldMap& GetScriptFieldMap(UUID entity_id);
    static MonoObject* GetEntityMonoScriptInstance(UUID entity_id);
    static Ref<ScriptInstance> GetEntityScriptInstance(UUID entity_id);
    static Ref<ScriptClass> GetEntityClass(const std::string& class_name);
    static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

  private:
    static void InitMono();
    static void ShutdownMono();
    static MonoObject* InstantiateClass(MonoClass* mono_class);
    static void LoadAssemblyClasses();

  private:
    friend class ScriptClass;
    friend class ScriptRegistry;
  };

  namespace Utilities
  {
    inline const char* FieldType_ScriptFieldTypeToString(ScriptFieldType script_field_type)
    {
      switch (script_field_type)
      {
      case ScriptFieldType::None:     return "None";
      case ScriptFieldType::Bool:     return "Bool";
      case ScriptFieldType::Float:    return "Float";
      case ScriptFieldType::Double:   return "Double";
      case ScriptFieldType::Decimal:  return "Decimal";
      case ScriptFieldType::Long:     return "Long";
      case ScriptFieldType::Int:      return "Int";
      case ScriptFieldType::Short:    return "Short";
      case ScriptFieldType::Byte:     return "Byte";
      case ScriptFieldType::ULong:    return "ULong";
      case ScriptFieldType::UInt:     return "UInt";
      case ScriptFieldType::UShort:   return "UShort";
      case ScriptFieldType::UByte:    return "UByte";
      case ScriptFieldType::Vector2:  return "Vector2";
      case ScriptFieldType::Vector3:  return "Vector3";
      case ScriptFieldType::Vector4:  return "Vector4";
      case ScriptFieldType::Entity:   return "Entity";
      default:                        return "None";
      }
    }

    inline ScriptFieldType FieldType_ScriptToScriptFieldType(std::string type_string)
    {
      if (type_string == "Bool")    return ScriptFieldType::Bool;
      if (type_string == "Float")   return ScriptFieldType::Float;
      if (type_string == "Double")  return ScriptFieldType::Double;
      if (type_string == "Decimal") return ScriptFieldType::Decimal;
      if (type_string == "Long")    return ScriptFieldType::Long;
      if (type_string == "Int")     return ScriptFieldType::Int;
      if (type_string == "Short")   return ScriptFieldType::Short;
      if (type_string == "Byte")    return ScriptFieldType::Byte;
      if (type_string == "ULong")   return ScriptFieldType::ULong;
      if (type_string == "UInt")    return ScriptFieldType::UInt;
      if (type_string == "UShort")  return ScriptFieldType::UShort;
      if (type_string == "UByte")   return ScriptFieldType::UByte;
      if (type_string == "Vector2") return ScriptFieldType::Vector2;
      if (type_string == "Vector3") return ScriptFieldType::Vector3;
      if (type_string == "Vector4") return ScriptFieldType::Vector4;
      if (type_string == "Entity")  return ScriptFieldType::Entity;
      
      return ScriptFieldType::None;
    }
  }

}