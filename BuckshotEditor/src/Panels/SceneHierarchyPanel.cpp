#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>
#include <entt.hpp>

#include "SceneHierarchyPanel.h"

namespace Buckshot {

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
  {
    SetContext(context);
  }

  void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
  {
    m_Context = context;
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
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

    if (ImGui::IsItemClicked())
    {
      m_SelectionContext = entity;
    }

    if (opened)
    {
      ImGui::TreePop();
    }
  }

  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    if (entity.HasComponent<TagComponent>())
    {
      auto& tag = entity.GetComponent<TagComponent>().Tag;

      char buffer[256];
      std::memset(buffer, 0, sizeof(buffer));
      std::strcpy(buffer, tag.c_str());
      if (ImGui::InputText(" ", buffer, sizeof(buffer)))
      {
        tag = std::string(buffer);
      }
      ImGui::Separator();
    }

    if (entity.HasComponent<TransformComponent>())
    {
      if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
      {
        auto& transform = entity.GetComponent<TransformComponent>().Transform;
        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);
      }
    }

    if (entity.HasComponent<CameraComponent>())
    {
      if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
      {
        auto& camera_component = entity.GetComponent<CameraComponent>();
        
        const char* projection_type_strings[] = { "Perspective", "Orthographic" };
        const char* current_projection_type_string = projection_type_strings[(int)camera_component.Camera.GetProjectionType()];

        ImGui::Checkbox("Primary", &camera_component.Primary);
        ImGui::Checkbox("Fixed Aspect Ratio", &camera_component.FixedAspectRatio);

        if (ImGui::BeginCombo("Projection", current_projection_type_string))
        {

          for (int i = 0; i < 2; i++)
          {
            bool is_selected = current_projection_type_string == projection_type_strings[i];
            if (ImGui::Selectable(projection_type_strings[i], is_selected))
            {
              current_projection_type_string = projection_type_strings[i];
              camera_component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
            }

            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }

        if (camera_component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
        {
          float size = camera_component.Camera.GetOrthographicSize();
          if (ImGui::DragFloat("Size", &size))
          {
            camera_component.Camera.SetOrthographicSize(size);
          }

          float near_clip = camera_component.Camera.GetOrthographicNearClip();
          if (ImGui::DragFloat("Near Clip", &near_clip))
          {
            camera_component.Camera.SetOrthographicNearClip(near_clip);
          }

          float far_clip = camera_component.Camera.GetOrthographicFarClip();
          if (ImGui::DragFloat("Far Clip", &far_clip))
          {
            camera_component.Camera.SetOrthographicFarClip(far_clip);
          }

        }

        if (camera_component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
        {
          float fov = glm::degrees(camera_component.Camera.GetPerspectiveFOV());
          if (ImGui::DragFloat("FOV", &fov))
          {
            camera_component.Camera.SetPerspectiveFOV(glm::radians(fov));
          }

          float near_clip = camera_component.Camera.GetPerspectiveNearClip();
          if (ImGui::DragFloat("Near Clip", &near_clip))
          {
            camera_component.Camera.SetPerspectiveNearClip(near_clip);
          }

          float far_clip = camera_component.Camera.GetPerspectiveFarClip();
          if (ImGui::DragFloat("Far Clip", &far_clip))
          {
            camera_component.Camera.SetPerspectiveFarClip(far_clip);
          }
        }

      }
    }
  }

}