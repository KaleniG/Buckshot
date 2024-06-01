#include <bspch.h>

#include "Buckshot/Project/ProjectSerializer.h"
#include "Buckshot/Project/Project.h"

namespace Buckshot {

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
      s_ActiveProject = project;
      s_ActiveProject->GetConfiguration().ProjectDirectory = filepath.parent_path();
      return s_ActiveProject;
    }
      

    return nullptr;
  }

  bool Project::Save(const std::filesystem::path& filepath)
  {
    ProjectSerializer serializer(s_ActiveProject);
    
    if (serializer.Serialize(filepath))
    {
      s_ActiveProject->GetConfiguration().ProjectDirectory = filepath.parent_path();
      return true;
    }

    return false;
  }

  const std::filesystem::path& Project::GetProjectDirectory()
  {
    BS_ASSERT(s_ActiveProject);
    return s_ActiveProject->GetConfiguration().ProjectDirectory;
  }

  std::filesystem::path Project::GetFileSystemAssetPath(const std::filesystem::path& filepath)
  {
    BS_ASSERT(s_ActiveProject);
    return GetAssetsDirectory() / filepath;
  }

  std::filesystem::path Project::GetAssetsDirectory()
  {
    BS_ASSERT(s_ActiveProject);
    return GetProjectDirectory() / s_ActiveProject->GetConfiguration().AssetsDirectory;
  }

}