#include <bspch.h>
#include <yaml-cpp/yaml.h>

#include "ProjectSerializer.h"

namespace Buckshot {

  ProjectSerializer::ProjectSerializer(const Ref<Project>& project)
    : m_Project(project)
  {

  }

  bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
  {
    const ProjectConfiguration& config = m_Project->GetConfiguration();

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Project" << YAML::Value;
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << config.Name;
    out << YAML::Key << "AssetsDirectory" << YAML::Value << config.AssetsDirectory.string();
    out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
    out << YAML::Key << "StartScenePath" << YAML::Value << config.StartScenePath.string();
    out << YAML::EndMap;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();

    return true; // Why?
  }

  bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    ProjectConfiguration& config = m_Project->GetConfiguration();

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

    YAML::Node project = data["Project"];
    if (!project)
      return false;

    config.Name = project["Name"].as<std::string>();
    config.AssetsDirectory = project["AssetsDirectory"].as<std::string>();
    config.ScriptModulePath = project["ScriptModulePath"].as<std::string>();
    config.StartScenePath = project["StartScenePath"].as<std::string>();

    return true;
  }

}