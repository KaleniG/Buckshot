#pragma once

#include <filesystem>

#include "Buckshot/Core/Base.h"

namespace Buckshot { 

  struct ProjectConfiguration
  {
    std::string Name = "Untitled";

    std::string StartScene;
    std::filesystem::path AssetDirectory;
    std::filesystem::path ScriptModulePath;
  };

  class Project
  {
  public:
    static Ref<Project> GetActive() { return s_ActiveProject; }
    static const std::filesystem::path& GetAssetDirectory();
    static const std::filesystem::path& GetProjectDirectory();
    static const std::filesystem::path& GetAssetFileSystemPath(const std::string& path);
    ProjectConfiguration& GetConfiguration() { return m_Configuration; }

    static Ref<Project> New();
    static Ref<Project> Load(const std::filesystem::path& filepath);
    static bool SaveActive(const std::filesystem::path& filepath);

  private:
    ProjectConfiguration m_Configuration;
    std::filesystem::path m_ProjectDirectory;
    inline static Ref<Project> s_ActiveProject;
  };

}