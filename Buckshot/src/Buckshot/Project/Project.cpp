#include <bspch.h>

#include "Buckshot/Project/ProjectSerializer.h"
#include "Buckshot/Project/Project.h"

namespace Buckshot {

  const std::filesystem::path& Project::GetAssetDirectory()
  {
    BS_ASSERT(s_ActiveProject);
    return s_ActiveProject->m_Configuration.AssetDirectory;
  }

  const std::filesystem::path& Project::GetProjectDirectory()
  {
    BS_ASSERT(s_ActiveProject);
    return s_ActiveProject->m_ProjectDirectory;
  }

  const std::filesystem::path& Project::GetAssetFileSystemPath(const std::string& path)
  {
    BS_ASSERT(s_ActiveProject);
    return GetProjectDirectory() / GetAssetDirectory() / path;
  }

  Ref<Project> Project::New()
  {
    s_ActiveProject = CreateRef<Project>();
    return s_ActiveProject;
  }

  Ref<Project> Project::Load(const std::filesystem::path& filepath)
  {
    Ref<Project> project = CreateRef<Project>();

    ProjectSerializer serializer(project);
    if (serializer.Deserialize(filepath))
    {
      project->m_ProjectDirectory = filepath.parent_path();
      s_ActiveProject = project;
      return s_ActiveProject;
    }

    return nullptr;
  }

  bool Project::SaveActive(const std::filesystem::path& filepath)
  {
    ProjectSerializer serializer(s_ActiveProject);
    s_ActiveProject->m_ProjectDirectory = filepath.parent_path();
    return serializer.Serialize(filepath);
  }

}