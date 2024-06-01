#pragma once

#include "Buckshot/Project/ProjectSerializer.h"
#include "Buckshot/Project/Project.h"

namespace Buckshot
{

  class ProjectSerializer
  {
  public:
    ProjectSerializer(const Ref<Project>& project);
    bool Serialize(const std::filesystem::path& filepath);
    bool Deserialize(const std::filesystem::path& filepath);

  private:
    Ref<Project> m_Project;
  };

}
