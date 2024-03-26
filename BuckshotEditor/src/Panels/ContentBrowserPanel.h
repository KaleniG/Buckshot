#pragma once

#include <bspch.h>

namespace Buckshot {

  class ContentBrowserPanel
  {
  public:
    ContentBrowserPanel();

    void OnImGuiRender();

  private:
    std::filesystem::path m_CurrentDirectory;
  };

}