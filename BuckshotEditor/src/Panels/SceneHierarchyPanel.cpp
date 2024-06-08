#include <bspch.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>

#include "SceneHierarchyPanel.h"

namespace Buckshot {

  static void DrawVec3Control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float column_width = 100.0f, bool is_radians = false)
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
    if (!is_radians)
      ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    else
    {
      float degrees = glm::degrees(values.x);
      ImGui::DragFloat("##X", &degrees, 0.1f, 0.0f, 0.0f, "%.2f");
      values.x = glm::radians(degrees);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushFont(bold_font);
    if (ImGui::Button("Y", button_size))
      values.y = reset_value;
    ImGui::PopFont();

    ImGui::SameLine();
    if (!is_radians)
      ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    else
    {
      float degrees = glm::degrees(values.y);
      ImGui::DragFloat("##Y", &degrees, 0.1f, 0.0f, 0.0f, "%.2f");
      values.y = glm::radians(degrees);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushFont(bold_font);
    if (ImGui::Button("Z", button_size))
      values.z = reset_value;
    ImGui::PopFont();

    ImGui::SameLine();
    if (!is_radians)
      ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    else
    {
      float degrees = glm::degrees(values.z);
      ImGui::DragFloat("##Z", &degrees, 0.1f, 0.0f, 0.0f, "%.2f");
      values.z = glm::radians(degrees);
    }
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
  }

  static void DrawVec2Control(const std::string& label, glm::vec2& values, float reset_value = 0.0f, float column_width = 100.0f)
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

  void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
  {
    m_SelectionContext = entity;
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

      if (ImGui::MenuItem("Duplicate"))
      {
        m_SelectionContext = m_Context->DuplicateEntity(entity);
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
      if (!entity.HasComponent<TransformComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Transform"))
        {
          m_SelectionContext.AddComponent<TransformComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

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

      if (!entity.HasComponent<CircleRendererComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Circle Renderer"))
        {
          m_SelectionContext.AddComponent<CircleRendererComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!entity.HasComponent<Rigidbody2DComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Rigidbody2D"))
        {
          m_SelectionContext.AddComponent<Rigidbody2DComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!entity.HasComponent<BoxCollider2DComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("BoxCollider2D"))
        {
          m_SelectionContext.AddComponent<BoxCollider2DComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!entity.HasComponent<CircleCollider2DComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("CircleCollider2D"))
        {
          m_SelectionContext.AddComponent<CircleCollider2DComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!entity.HasComponent<ScriptComponent>())
      {
        present_any_component = true;
        if (ImGui::MenuItem("Script"))
        {
          m_SelectionContext.AddComponent<ScriptComponent>();
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
        ImGui::Separator();
        DrawVec3Control("Position", component.Position);
        DrawVec3Control("Rotation", component.Rotation, 0.0f, 100.0f, true);
        DrawVec3Control("Scale", component.Scale, 1.0f);
        ImGui::Separator();
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

        ImGui::Button("Texture");
        if (ImGui::BeginDragDropTarget())
        {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_FILE"))
          {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::filesystem::path texture_path(path);
            component.Texture = Texture2D::Create(texture_path.string());
          }
          ImGui::EndDragDropTarget();
        }
        if (component.Texture)
          ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
      });

    DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
      {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
        ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
      });

    DrawComponent<Rigidbody2DComponent>("Rigidbody2D", entity, [](auto& component)
      {
        const char* body_type_strings[] = { "Static", "Dynamic", "Kinematic" };
        const char* current_body_type_string = body_type_strings[(int)component.Type];

        if (ImGui::BeginCombo("Body Type", current_body_type_string))
        {
          for (int i = 0; i < 3; i++)
          {
            bool isSelected = current_body_type_string == body_type_strings[i];
            if (ImGui::Selectable(body_type_strings[i], isSelected))
            {
              current_body_type_string = body_type_strings[i];
              component.Type = (Rigidbody2DComponent::BodyType)i;
            }

            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }

          ImGui::EndCombo();
        }

        ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);

      });

    DrawComponent<BoxCollider2DComponent>("BoxCollider2D", entity, [](auto& component)
      {
        ImGui::Separator();
        DrawVec2Control("Size", component.Size, 0.5f);
        DrawVec2Control("Offset", component.Offset);
        ImGui::Separator();

        ImGui::DragFloat("Density", &component.Density, 0.1f);
        ImGui::DragFloat("Friction", &component.Friction, 0.1f);
        ImGui::DragFloat("Restituition", &component.Restituition, 0.1f);
        ImGui::DragFloat("RestituitionThreshold", &component.RestituitionThreshold, 0.1f);
      });

    DrawComponent<CircleCollider2DComponent>("CircleCollider2D", entity, [](auto& component)
      {
        ImGui::Separator();
        DrawVec2Control("Offset", component.Offset);
        ImGui::Separator();

        ImGui::DragFloat("Radius", &component.Radius, 0.1f);
        ImGui::DragFloat("Density", &component.Density, 0.1f);
        ImGui::DragFloat("Friction", &component.Friction, 0.1f);
        ImGui::DragFloat("Restituition", &component.Restituition, 0.1f);
        ImGui::DragFloat("RestituitionThreshold", &component.RestituitionThreshold, 0.1f);
      });

    DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
      {
        bool script_class_exists = ScriptEngine::EntityClassExists(component.Name);

        static char buffer[64];
        std::strcpy(buffer, component.Name.c_str());

        UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.85f, 0.2f, 0.2f, 1.0f), !script_class_exists);

        if (ImGui::InputText("Class", buffer, sizeof(buffer) / sizeof(char)))
        {
          component.Name = buffer;
          return;
        }

        bool scene_running = scene->IsRunning();

        if (scene_running)
        {
          Ref<ScriptInstance> script_instance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
          if (script_instance)
          {
            const auto& fields = script_instance->GetScriptClass()->GetScriptFields();
            for (const auto& [name, field] : fields)
            {
              if (field.Type == ScriptFieldType::Float)
              {
                float data = script_instance->GetFieldValue<float>(name);
                if (ImGui::DragFloat(name.c_str(), &data))
                {
                  script_instance->SetFieldValue(name, data);
                }
              }
            }
          }
        }
        else
        {
          if (script_class_exists)
          {
            Ref<ScriptClass> entity_class = ScriptEngine::GetEntityClass(component.Name);
            const auto& class_fields = entity_class->GetScriptFields();

            auto& entity_fields = ScriptEngine::GetScriptFieldMap(entity.GetUUID());
            for (const auto& [name, field] : class_fields)
            {
              if (entity_fields.find(name) != entity_fields.end())
              {
                ScriptFieldInstance& script_field = entity_fields.at(name);

                if (field.Type == ScriptFieldType::Float)
                {
                  float data = script_field.GetValue<float>();
                  if (ImGui::DragFloat(name.c_str(), &data))
                    script_field.SetValue(data);
                }
              }
              else
              {
                if (field.Type == ScriptFieldType::Float)
                {
                  float data = 0.0f;
                  if (ImGui::DragFloat(name.c_str(), &data))
                  {
                    ScriptFieldInstance& field_instance = entity_fields[name];
                    field_instance.Field = field;
                    field_instance.SetValue(data);
                  }
                }
              }
            }
          }
        }
      });

    // TODO: NativeScriptComponent

    ImGui::PopStyleVar();
  }

}