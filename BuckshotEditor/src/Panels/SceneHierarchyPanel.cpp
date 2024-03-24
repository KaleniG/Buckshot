#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>

#include "SceneHierarchyPanel.h"

namespace Buckshot {

  static void DrawVec3Control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float column_width = 100.0f)
  {
    ImGuiIO& io = ImGui::GetIO();
    auto bold_font = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0.0f, column_width);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 button_size = ImVec2(line_height + 3.0f, line_height);

    ImGui::PushFont(bold_font);
    if (ImGui::Button("X", button_size))
      values.x = reset_value;
    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushFont(bold_font);
    if (ImGui::Button("Y", button_size))
      values.y = reset_value;
    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushFont(bold_font);
    if (ImGui::Button("Z", button_size))
      values.z = reset_value;
    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
  }

  template<typename T, typename UIFunction>
  static void DrawComponent(const std::string& label, Entity entity, UIFunction ui_function)
  {
    const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if (entity.HasComponent<T>())
    {
      auto& component = entity.GetComponent<T>();

      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
      float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
      ImGui::Separator();
      bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_node_flags, label.c_str());
      ImGui::PopStyleVar();

      ImGui::SameLine(ImGui::GetContentRegionAvail().x - line_height * 0.5f);

      if (ImGui::Button("+", ImVec2(line_height, line_height)))
      {
        ImGui::OpenPopup("ComponentSettings");
      }

      bool remove_component = false;
      if (ImGui::BeginPopup("ComponentSettings"))
      {
        if (ImGui::MenuItem("Remove Component"))
        {
          remove_component = true;
        }

        ImGui::EndPopup();
      }

      if (open)
      {
        ui_function(component);
        ImGui::TreePop();
      }

      if (remove_component)
      {
        entity.RemoveComponent<T>();
      }
    }
  }

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
  {
    SetContext(context);
  }

  void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
  {
    m_Context = context;
    m_SelectionContext = {};
  }

  void SceneHierarchyPanel::OnImGuiRender()
  {
    ImGui::Begin("Scene Hierarchy");

    m_Context->m_Registry.each([&](auto entity_id)
    {
      Entity entity = Entity(entity_id, m_Context.get());
      DrawEntityNode(entity);
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
      m_SelectionContext = {};

    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
    {
      if (ImGui::MenuItem("New Entity"))
      {
        m_Context->CreateEntity("NewEntity");
      }

      ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_SelectionContext)
    {
      DrawComponents(m_SelectionContext);
    }

    ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntityNode(Entity entity)
  {
    auto& tag = entity.GetComponent<TagComponent>().Tag;
    
    ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

    if (ImGui::IsItemClicked())
    {
      m_SelectionContext = entity;
    }

    bool entity_deleted = false;
    if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
    {
      if (ImGui::MenuItem("Delete"))
      {
        entity_deleted = true;
      }

      ImGui::EndPopup();
    }

    if (opened)
    {
      ImGui::TreePop();
    }

    if (entity_deleted)
    {
      if (m_SelectionContext == entity)
        m_SelectionContext = {};
      m_Context->DestroyEntity(entity);
    }
  }

  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

    if (entity.HasComponent<TagComponent>())
    {
      auto& tag = entity.GetComponent<TagComponent>().Tag;

      char buffer[256];
      std::memset(buffer, 0, sizeof(buffer));
      std::strncpy(buffer, tag.c_str(), sizeof(buffer));
      if (ImGui::InputText("##TAG", buffer, sizeof(buffer)))
      {
        tag = std::string(buffer);
      }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
    {
      ImGui::OpenPopup("AddComponent");
    }

    bool present_any_component = false;
    if (ImGui::BeginPopup("AddComponent"))
    {
      if (!entity.HasComponent<CameraComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Camera"))
        {
          m_SelectionContext.AddComponent<CameraComponent>();
          m_Context->OnViewportResize();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!entity.HasComponent<SpriteRendererComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Sprite Renderer"))
        {
          m_SelectionContext.AddComponent<SpriteRendererComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!present_any_component)
      {
        ImGui::Text("No Components Available");
      }

      ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
    {
        DrawVec3Control("Position", component.Position);
        DrawVec3Control("Rotation", component.Rotation);
        DrawVec3Control("Scale", component.Scale, 1.0f);
    });

    DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
    {
      const char* projection_type_strings[] = { "Perspective", "Orthographic" };
      const char* current_projection_type_string = projection_type_strings[(int)component.Camera.GetProjectionType()];

      ImGui::Checkbox("Primary", &component.Primary);

      if (ImGui::BeginCombo("Projection", current_projection_type_string))
      {
        for (int i = 0; i < 2; i++)
        {
          bool isSelected = current_projection_type_string == projection_type_strings[i];
          if (ImGui::Selectable(projection_type_strings[i], isSelected))
          {
            current_projection_type_string = projection_type_strings[i];
            component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
      {
        float fov = glm::degrees(component.Camera.GetPerspectiveFOV());
        if (ImGui::DragFloat("FOV", &fov, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetPerspectiveFOV(glm::radians(fov));
        }

        float near_clip = component.Camera.GetPerspectiveNearClip();
        if (ImGui::DragFloat("Near Clip", &near_clip, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetPerspectiveNearClip(near_clip);
        }

        float far_clip = component.Camera.GetPerspectiveFarClip();
        if (ImGui::DragFloat("Far Clip", &far_clip, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetPerspectiveFarClip(far_clip);
        }
      }

      if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
      {
        ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

        float size = component.Camera.GetOrthographicSize();
        if (ImGui::DragFloat("Size", &size, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetOrthographicSize(size);
        }

        float near_clip = component.Camera.GetOrthographicNearClip();
        if (ImGui::DragFloat("Near Clip", &near_clip, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetOrthographicNearClip(near_clip);
        }

        float far_clip = component.Camera.GetOrthographicFarClip();
        if (ImGui::DragFloat("Far Clip", &far_clip, 1.0f, 0.0f, 0.0f, "%.0f"))
        {
          component.Camera.SetOrthographicFarClip(far_clip);
        }
      }
    });

    DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
    });

    ImGui::PopStyleVar();
  }

}