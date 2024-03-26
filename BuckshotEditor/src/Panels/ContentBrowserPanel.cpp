#include <bspch.h>
#include <ImGui/imgui.h>

#include "ContentBrowserPanel.h"

namespace Buckshot {

  static const std::filesystem::path s_AssetsPath("assets");

  ContentBrowserPanel::ContentBrowserPanel()
  {
    m_CurrentDirectory = s_AssetsPath;
  }

  void ContentBrowserPanel::OnImGuiRender()
  {
    ImGui::Begin("Content Browser");
    
    if (m_CurrentDirectory != s_AssetsPath)
    {
      if (ImGui::Button("<-"))
      {
        m_CurrentDirectory = m_CurrentDirectory.parent_path();
      }
    }

    for (auto& directory_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
      const auto& path = directory_entry.path();
      auto relative_path = std::filesystem::relative(path, s_AssetsPath);
      std::string filename_string = relative_path.filename().string();
      if (directory_entry.is_directory())
      {
        if (ImGui::Button(filename_string.c_str()))
        {
          m_CurrentDirectory /= directory_entry.path().filename();
        }
      }
      else
      {
        if (ImGui::Button(filename_string.c_str()))
        {
        }
      }
    }

    ImGui::End();
  }

}