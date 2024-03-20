#pragma once

#include <Buckshot.h>

namespace Buckshot {

  class SceneHierarchyPanel
  {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

  private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity entity);
    void DrawVec3Control(const std::string& label, glm::vec3& vector, float reset_value = 0.0f, float column_width = 100.0f);

  private:
    Ref<Scene> m_Context;
    Entity m_SelectionContext;
  };

}