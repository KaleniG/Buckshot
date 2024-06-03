#pragma once

#include <ImGui/imgui.h>

namespace Buckshot::UI {

  class ScopedStyleColor
  {
  public:
    ScopedStyleColor() = default;

    ScopedStyleColor(ImGuiCol idx, const ImVec4& col, bool predicate = true)
      : m_Set(predicate)
    {
      if (predicate)
        ImGui::PushStyleColor(idx, col);
    }

    ScopedStyleColor(ImGuiCol idx, ImU32 col, bool predicate = true)
      : m_Set(predicate)
    {
      if (predicate)
        ImGui::PushStyleColor(idx, col);
    }

    ~ScopedStyleColor()
    {
      if (m_Set)
        ImGui::PopStyleColor();
    }

  private:
    bool m_Set;
  };

}
