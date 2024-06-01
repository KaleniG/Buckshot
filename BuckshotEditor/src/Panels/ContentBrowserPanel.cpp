#include <bspch.h>
#include <ImGui/imgui.h>

#include "ContentBrowserPanel.h"

namespace Buckshot {

  ContentBrowserPanel::ContentBrowserPanel()
    : m_BaseDirectory(Project::GetAssetsDirectory()), m_CurrentDirectory(m_BaseDirectory)
  {
    m_DirectoryIcon = Texture2D::Create("assets/textures/DirectoryIcon.png");
    m_FileIcon = Texture2D::Create("assets/textures/FileIcon.png");
  }

  void ContentBrowserPanel::OnImGuiRender()
  {
    ImGui::Begin("Content Browser");

    if (m_CurrentDirectory != m_BaseDirectory)
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

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    for (auto& directory_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
      const auto& path = directory_entry.path();
      std::string filename_string = path.filename().string();

      ImGui::PushID(filename_string.c_str());

      Ref<Texture2D> icon = directory_entry.is_directory() ? m_DirectoryIcon : m_FileIcon;
      ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

      if (ImGui::BeginDragDropSource())
      {
        std::filesystem::path relative_path(path);
        const wchar_t* item_path = relative_path.c_str();
        ImGui::SetDragDropPayload("ASSET_BROWSER_FILE", item_path, (std::wcslen(item_path) + 1) * sizeof(wchar_t), ImGuiCond_Once);
        ImGui::EndDragDropSource();
      }

      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      {
        if (directory_entry.is_directory())
          m_CurrentDirectory /= path.filename();
      }

      ImGui::TextWrapped(filename_string.c_str());

      ImGui::NextColumn();
      ImGui::PopID();
    }
    ImGui::PopStyleColor();

    ImGui::Columns(1);

    // EX
    // ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);
    // 
    ImGui::End();
  }

}