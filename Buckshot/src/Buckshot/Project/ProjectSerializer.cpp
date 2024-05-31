#include <bspch.h>
#include <yaml-cpp/yaml.h>

#include "Buckshot/Project/ProjectSerializer.h"

namespace Buckshot {

  ProjectSerializer::ProjectSerializer(Ref<Project> project)
  {
    m_Project = project;
  }

  bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
  {
    const auto& configuration = m_Project->GetConfiguration();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;

		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << configuration.Name;
		out << YAML::Key << "StartScene" << YAML::Value << configuration.StartScene;
		out << YAML::Key << "AssetDirectory" << YAML::Value << configuration.AssetDirectory.string();
		out << YAML::Key << "ScriptModulePath" << YAML::Value << configuration.ScriptModulePath.string();
		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

    return true;
  }

  bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    auto& configuration = m_Project->GetConfiguration();

    YAML::Node data;

    try
    {
      data = YAML::LoadFile(filepath.string());
    }
    catch (YAML::ParserException e)
    {
      BS_TRACE("Failed to load the .bproj file \"{0}\". {1}", filepath, e.what());
      return false;
    }

    auto project_data = data["Project"];
    if (!project_data)
      return false;

    configuration.Name = project_data["Name"].as<std::string>();
    configuration.StartScene = project_data["StartScene"].as<std::string>();
    configuration.AssetDirectory = project_data["AssetDirectory"].as<std::string>();
    configuration.ScriptModulePath = project_data["ScriptModulePath"].as<std::string>();

    return true;
  }

}