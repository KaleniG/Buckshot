#pragma once

#include <bspch.h>
#include <Buckshot.h>

namespace Buckshot {

  class ContentBrowserPanel
  {
  public:
    ContentBrowserPanel();

    void OnImGuiRender();

  private:
    std::filesystem::path m_CurrentDirectory;

    Ref<Texture2D> m_DirectoryIcon;
    Ref<Texture2D> m_FileIcon;
  };

}