#pragma once

#include <filesystem>

#include "Buckshot/Core/Base.h" 

namespace Buckshot {

  struct ProjectConfiguration
  {
    // SERIALIZED
    std::string Name = "Untitled";
    std::filesystem::path AssetsDirectory;
    std::filesystem::path ScriptModulePath;
    std::filesystem::path StartScenePath;

    // NOT SERIALIZED
    std::filesystem::path ProjectDirectory;
  };

  class Project
  {
  public:
    static Ref<Project> New();
    static Ref<Project> Load(const std::filesystem::path& filepath);
    static bool Save(const std::filesystem::path& filepath);

    static Ref<Project> GetActive() { return s_ActiveProject; }
    static const std::filesystem::path& GetProjectDirectory();
    static std::filesystem::path GetFileSystemAssetPath(const std::filesystem::path& filepath);
    static std::filesystem::path GetAssetsDirectory();

    ProjectConfiguration& GetConfiguration() { return m_Configuration; }
 
  private:
    ProjectConfiguration m_Configuration;
    inline static Ref<Project> s_ActiveProject;
  };

}