#include <bspch.h>
#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>

#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scripting/ScriptEngine.h"
#include "Buckshot/Scripting/ScriptRegistry.h"

namespace Buckshot {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap = {
		{"System.Boolean", ScriptFieldType::Bool},
		{"System.Single", ScriptFieldType::Float},
		{"System.Double", ScriptFieldType::Double},
		{"System.Decimal", ScriptFieldType::Decimal},
		{"System.Int64", ScriptFieldType::Long},
		{"System.Int32", ScriptFieldType::Int},
		{"System.Int16", ScriptFieldType::Short},
		{"System.SByte", ScriptFieldType::Byte},
		{"System.UInt64", ScriptFieldType::ULong},
		{"System.UInt32", ScriptFieldType::UInt},
		{"System.UInt16", ScriptFieldType::UShort},
		{"System.Byte", ScriptFieldType::UByte},
		{"Buckshot.Vector2", ScriptFieldType::Vector2},
		{"Buckshot.Vector3", ScriptFieldType::Vector3},
		{"Buckshot.Vector4", ScriptFieldType::Vector4},
		{"Buckshot.Entity", ScriptFieldType::Entity}
	};

  namespace Utilities {

		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadCSharpAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				return nullptr;
			}

			std::string path_string = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, path_string.c_str(), &status, 0);
			mono_image_close(image);

			delete[] fileData;

			return assembly;
		}

		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				BS_TRACE("{0}.{1}", nameSpace, name);
			}
		}

		static ScriptFieldType FieldType_MonoToBuckshot(MonoType* mono_type)
		{
			std::string mono_type_name = mono_type_get_name(mono_type);

			BS_ASSERT(s_ScriptFieldTypeMap.find(mono_type_name) != s_ScriptFieldTypeMap.end(), "Invalid MonoType-to-ScriptFieldType conversion, invalid type: {0}", mono_type_name);

			return s_ScriptFieldTypeMap.at(mono_type_name);
		}

  }

  struct ScriptEngineData
  {
    MonoDomain* RootDomain = nullptr;
    MonoDomain* AppDomain = nullptr;

    MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scene* SceneContext = nullptr;
  };

  static ScriptEngineData* s_Data = nullptr;

	/////////////////////////////////
	// SCRIPT ENGINE ////////////////
	/////////////////////////////////

  void ScriptEngine::Init()
  {
    s_Data = new ScriptEngineData();

    InitMono();
		LoadAssembly("scripts/Buckshot-ScriptCore.dll");
		LoadAppAssembly("scripts/Sandbox.dll");
		LoadAssemblyClasses();
		
		s_Data->EntityClass = ScriptClass("Buckshot", "Entity", true);

		ScriptRegistry::RegisterComponents();
		ScriptRegistry::RegisterFunctions();
  }

  void ScriptEngine::Shutdown()
  {
    ShutdownMono();
    delete s_Data;
  }

  void ScriptEngine::InitMono()
  {
    mono_set_assemblies_path("mono/lib");

		// Setting root domain
		MonoDomain* rootDomain = mono_jit_init("BuckshotJITRuntime");
		BS_ASSERT(rootDomain, "No root domain");
		s_Data->RootDomain = rootDomain;
  }

	void ScriptEngine::ShutdownMono()
	{
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnCreateEntity(Entity& entity)
	{
		const auto& script_component = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(script_component.Name))
		{
			UUID entity_id = entity.GetUUID();

			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[script_component.Name], entity);
			s_Data->EntityInstances[entity_id] = instance;

			if (s_Data->EntityScriptFields.find(entity_id) != s_Data->EntityScriptFields.end())
			{
				const ScriptFieldMap& field_map = s_Data->EntityScriptFields.at(entity_id);
				for (const auto& [name, field_instance] : field_map)
					instance->SetFieldValueInternal(name, (void*)field_instance.m_DataBuffer);
			}
			
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity& entity, Timestep timestep)
	{
		UUID entity_uuid = entity.GetUUID();
		BS_ASSERT(s_Data->EntityInstances.find(entity_uuid) != s_Data->EntityInstances.end(), "Invalid Entity");

		Ref<ScriptInstance> instance = s_Data->EntityInstances[entity_uuid];
		instance->InvokeOnUpdate(timestep);
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain(const_cast<char*>("BuckshotScriptRuntime"), nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utilities::LoadCSharpAssembly(filepath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = Utilities::LoadCSharpAssembly(filepath);
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
	}

	bool ScriptEngine::EntityClassExists(const std::string& full_name)
	{
		return s_Data->EntityClasses.find(full_name) != s_Data->EntityClasses.end();
	}
	
	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(UUID entity_id)
	{
		return s_Data->EntityScriptFields[entity_id];
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& class_name)
	{
		if (s_Data->EntityClasses.find(class_name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(class_name);
	}

	MonoObject* ScriptEngine::GetEntityMonoScriptInstance(UUID entity_id)
	{
		BS_ASSERT(s_Data->EntityInstances.find(entity_id) != s_Data->EntityInstances.end(), "Error");
		return s_Data->EntityInstances.at(entity_id)->GetScriptInstance();
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entity_id)
	{
		auto it = s_Data->EntityInstances.find(entity_id);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* mono_class)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, mono_class);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entity_class = mono_class_from_name(s_Data->CoreAssemblyImage, "Buckshot", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* name_space = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* class_name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string full_name;
			if (std::strlen(name_space) != 0)
				full_name = fmt::format("{0}.{1}", name_space, class_name);
			else
				full_name = class_name;

			MonoClass* mono_class = mono_class_from_name(s_Data->AppAssemblyImage, name_space, class_name);

			if (mono_class == entity_class)
				continue;

			bool is_subclass_of_entity = mono_class_is_subclass_of(mono_class, entity_class, false);

			if (!is_subclass_of_entity)
				continue;

			Ref<ScriptClass> script_class = CreateRef<ScriptClass>(name_space, class_name);
			s_Data->EntityClasses[full_name] = script_class;

			BS_WARN("{0} fields:", full_name);
			void* iterator = nullptr;
			MonoClassField* field;
			while ((field = mono_class_get_fields(mono_class, &iterator)))
			{
				std::string field_name = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type  = mono_field_get_type(field);
					ScriptFieldType script_field_type = Utilities::FieldType_MonoToBuckshot(type);
					BS_WARN("- {0} {1}", Utilities::FieldType_ScriptFieldTypeToString(script_field_type), field_name);

					script_class->m_Fields[field_name] = ScriptField(field_name, script_field_type, field);
				}
			}
		}
	}

	/////////////////////////////////
	// SCRIPT CLASS /////////////////
	/////////////////////////////////

	ScriptClass::ScriptClass(const std::string& class_namespace, const std::string& class_name, bool is_core)
	{
		m_ClassNamespace = class_namespace;
		m_ClassName = class_name;

		m_Class = mono_class_from_name(is_core ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, class_namespace.c_str(), class_name.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_Class);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameter_count)
	{
		return mono_class_get_method_from_name(m_Class, name.c_str(), parameter_count);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** parameters)
	{
		return mono_runtime_invoke(method, instance, parameters, nullptr);
	}

	/////////////////////////////////
	// SCRIPT INSTANCE //////////////
	/////////////////////////////////

	ScriptInstance::ScriptInstance(Ref<ScriptClass> script_class, Entity entity)
	{
		m_ScriptClass = script_class;
		m_Instance = script_class->Instantiate();
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = script_class->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = script_class->GetMethod("OnUpdate", 1);

		// Calling entity constructor
		UUID entityID = entity.GetUUID();
		void* param = &entityID;
		m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(Timestep timestep)
	{
		if (m_OnUpdateMethod)
		{
			void* parameter = &timestep;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &parameter);
		}
	}

	Ref<ScriptClass> ScriptInstance::GetScriptClass()
	{
		return m_ScriptClass;
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetScriptFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.MonoField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetScriptFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.MonoField, const_cast<void*>(value));
		return true;
	}

}