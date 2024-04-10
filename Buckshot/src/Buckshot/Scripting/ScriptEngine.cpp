#include <bspch.h>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>

#include "Buckshot/Scripting/ScriptEngine.h"

namespace Buckshot {

  namespace Utilities {

		char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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

		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
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
  };

  static ScriptEngineData* s_Data = nullptr;

  void ScriptEngine::Init()
  {
    s_Data = new ScriptEngineData();

    InitMono();
  }

  void ScriptEngine::Shutdown()
  {
    ShutdownMono();
    delete s_Data;
  }

  void ScriptEngine::InitMono()
  {
    mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("BuckshotJITRuntime");
		BS_ASSERT(rootDomain, "No root domain");
		s_Data->RootDomain = rootDomain;

		const char* name = "BuckshotScriptRuntime";
		s_Data->AppDomain = mono_domain_create_appdomain(const_cast<char*>(name), nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utilities::LoadCSharpAssembly("scripts/Buckshot-ScriptCore.dll");
		Utilities::PrintAssemblyTypes(s_Data->CoreAssembly);

		MonoImage* core_image = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* core_class = mono_class_from_name(core_image, "Buckshot", "Main");
		MonoObject* instance = mono_object_new(s_Data->AppDomain, core_class);

		MonoMethod* method_1 = mono_class_get_method_from_name(core_class, "PrintMessage", 0);
		MonoMethod* method_2 = mono_class_get_method_from_name(core_class, "PrintMessageEx", 1);

		mono_runtime_object_init(instance);
		mono_runtime_invoke(method_1, instance, nullptr, nullptr);

		MonoString* value = mono_string_new(s_Data->AppDomain, "HELLO");
		void* params[1] =
		{
			value
		};
		mono_runtime_invoke(method_2, instance, params, nullptr);
  }

	void ScriptEngine::ShutdownMono()
	{
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

}