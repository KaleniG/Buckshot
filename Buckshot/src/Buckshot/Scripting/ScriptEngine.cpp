#include <bspch.h>
#include <glm/glm.hpp>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>

#include "Buckshot/Scripting/ScriptEngine.h"
#include "Buckshot/Scripting/ScriptRegistry.h"

namespace Buckshot {

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

  }

  struct ScriptEngineData
  {
    MonoDomain* RootDomain = nullptr;
    MonoDomain* AppDomain = nullptr;

    MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass EntityClass;
  };

  static ScriptEngineData* s_Data = nullptr;

  void ScriptEngine::Init()
  {
    s_Data = new ScriptEngineData();

    InitMono();
		LoadAssembly("scripts/Buckshot-ScriptCore.dll");

		ScriptRegistry::RegisterFunctions();

		// Retrieving and instantiating class
		s_Data->EntityClass = ScriptClass("Buckshot", "Entity");
		MonoObject* instance = s_Data->EntityClass.Instantiate();

		// Invoking functions
		MonoMethod* print_message_method = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		s_Data->EntityClass.InvokeMethod(instance, print_message_method);
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

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain(const_cast<char*>("BuckshotScriptRuntime"), nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utilities::LoadCSharpAssembly(filepath);
		Utilities::PrintAssemblyTypes(s_Data->CoreAssembly);

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* mono_class)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, mono_class);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

	ScriptClass::ScriptClass(const std::string& class_namespace, const std::string& class_name)
	{
		m_ClassNamespace = class_namespace;
		m_ClassName = class_name;

		m_Class = mono_class_from_name(s_Data->CoreAssemblyImage, class_namespace.c_str(), class_name.c_str());
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

}