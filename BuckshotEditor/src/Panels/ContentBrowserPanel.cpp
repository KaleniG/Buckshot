#include <bspch.h>
#include <ImGui/imgui.h>

#include "ContentBrowserPanel.h"

namespace Buckshot {

  static const std::filesystem::path s_AssetsPath("assets");

  ContentBrowserPanel::ContentBrowserPanel()
  {
    m_CurrentDirectory = s_AssetsPath;
    m_DirectoryIcon = Texture2D::Create("assets/textures/DirectoryIcon.png");
    m_FileIcon = Texture2D::Create("assets/textures/FileIcon.png");
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

    static float padding = 16.0f;
    static float thumbnailSize = 128.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
      columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto& directory_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
      const auto& path = directory_entry.path();
      auto relative_path = std::filesystem::relative(path, s_AssetsPath);
      std::string filename_string = relative_path.filename().string();

      Ref<Texture2D> icon = directory_entry.is_directory() ? m_DirectoryIcon : m_FileIcon;
      ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      {
        if (directory_entry.is_directory())
          m_CurrentDirectory /= path.filename();
      }
      
      ImGui::TextWrapped(filename_string.c_str());

      ImGui::NextColumn();
    }

    ImGui::Columns(1);

    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);

    ImGui::End();
  }

}