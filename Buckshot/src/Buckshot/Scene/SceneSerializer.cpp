#include <bspch.h>
#include <yaml-cpp/yaml.h>

#include "Buckshot/Core/UUID.h"
#include "Buckshot/Scene/SceneSerializer.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scripting/ScriptEngine.h"

namespace YAML {

  template<>
  struct convert<glm::vec2>
  {
    static Node encode(const glm::vec2& rhs)
    {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      return node;
    }

    static bool decode(const Node& node, glm::vec2& rhs)
    {
      if (!node.IsSequence() || node.size() != 2)
        return false;

      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      return true;
    }
  };

  template<>
  struct convert<glm::vec3>
  {
    static Node encode(const glm::vec3& rhs)
    {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.push_back(rhs.z);
      return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs)
    {
      if (!node.IsSequence() || node.size() != 3)
        return false;

      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      rhs.z = node[2].as<float>();
      return true;
    }
  };

  template<>
  struct convert<glm::vec4>
  {
    static Node encode(const glm::vec4& rhs)
    {
      Node node;
      node.push_back(rhs.x);
      node.push_back(rhs.y);
      node.push_back(rhs.z);
      node.push_back(rhs.w);
      return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs)
    {
      if (!node.IsSequence() || node.size() != 4)
        return false;

      rhs.x = node[0].as<float>();
      rhs.y = node[1].as<float>();
      rhs.z = node[2].as<float>();
      rhs.w = node[3].as<float>();
      return true;
    }
  };

  template<>
  struct convert<Buckshot::UUID>
  {
    static Node encode(const Buckshot::UUID& uuid)
    {
      Node node;
      node.push_back((uint64_t)uuid);
      return node;
    }

    static bool decode(const Node& node, Buckshot::UUID& uuid)
    {
      uuid = node.as<uint64_t>();
      return true;
    }
  };

}

namespace Buckshot {
  
  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
  {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
  }

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
  {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
  }

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
  {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
  }

  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
    : m_Scene(scene) {}

  static void SerializeEntity(YAML::Emitter& out, Entity entity)
  {
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

    if (entity.HasComponent<TagComponent>())
    {
      out << YAML::Key << "TagComponent";
      out << YAML::BeginMap; // TagComponent

      auto& tag = entity.GetComponent<TagComponent>().Tag;
      out << YAML::Key << "Tag" << YAML::Value << tag;

      out << YAML::EndMap; // TagComponent
    }

    if (entity.HasComponent<TransformComponent>())
    {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap; // TransformComponent

      auto& tc = entity.GetComponent<TransformComponent>();
      out << YAML::Key << "Position" << YAML::Value << tc.Position;
      out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
      out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

      out << YAML::EndMap; // TransformComponent
    }

    if (entity.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap; // CameraComponent

      auto& cameraComponent = entity.GetComponent<CameraComponent>();
      auto& camera = cameraComponent.Camera;

      out << YAML::Key << "Camera" << YAML::Value;
      out << YAML::BeginMap; // Camera
      out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
      out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
      out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
      out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
      out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
      out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
      out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
      out << YAML::EndMap; // Camera

      out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
      out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

      out << YAML::EndMap; // CameraComponent
    }

    if (entity.HasComponent<SpriteRendererComponent>())
    {
      out << YAML::Key << "SpriteRendererComponent";
      out << YAML::BeginMap; // SpriteRendererComponent

      auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
      out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
      out << YAML::Key << "HasTexture" << YAML::Value << (bool)spriteRendererComponent.Texture;
      if (spriteRendererComponent.Texture)
      {
        out << YAML::Key << "Path" << YAML::Value << spriteRendererComponent.Texture->GetSourcePath();
        out << YAML::Key << "Tiling Factor" << YAML::Value << spriteRendererComponent.TilingFactor;
      }

      out << YAML::EndMap; // SpriteRendererComponent
    }

    if (entity.HasComponent<CircleRendererComponent>())
    {
      out << YAML::Key << "CircleRendererComponent";
      out << YAML::BeginMap; // CircleRendererComponent

      auto& circle_renderer_component = entity.GetComponent<CircleRendererComponent>();
      out << YAML::Key << "Color" << YAML::Value << circle_renderer_component.Color;
      out << YAML::Key << "Thickness" << YAML::Value << circle_renderer_component.Thickness;
      out << YAML::Key << "Fade" << YAML::Value << circle_renderer_component.Fade;

      out << YAML::EndMap; // CircleRendererComponent
    }

    if (entity.HasComponent<Rigidbody2DComponent>())
    {
      out << YAML::Key << "Rigidbody2DComponent";
      out << YAML::BeginMap; // Rigidbody2DComponent

      auto& rb2d_component = entity.GetComponent<Rigidbody2DComponent>();
      out << YAML::Key << "Type" << YAML::Value << (int)rb2d_component.Type;
      out << YAML::Key << "Fixed Rotation" << YAML::Value << rb2d_component.FixedRotation;

      out << YAML::EndMap; // Rigidbody2DComponent
    }

    if (entity.HasComponent<BoxCollider2DComponent>())
    {
      out << YAML::Key << "BoxCollider2DComponent";
      out << YAML::BeginMap; // BoxCollider2DComponent

      auto& bc2d_component = entity.GetComponent<BoxCollider2DComponent>();
      out << YAML::Key << "Offset" << YAML::Value << bc2d_component.Offset;
      out << YAML::Key << "Size" << YAML::Value << bc2d_component.Size;
      out << YAML::Key << "Density" << YAML::Value << bc2d_component.Density;
      out << YAML::Key << "Friction" << YAML::Value << bc2d_component.Friction;
      out << YAML::Key << "Restituition" << YAML::Value << bc2d_component.Restituition;
      out << YAML::Key << "Restituition Threshold" << YAML::Value << bc2d_component.RestituitionThreshold;

      out << YAML::EndMap; // BoxCollider2DComponent
    }

    if (entity.HasComponent<CircleCollider2DComponent>())
    {
      out << YAML::Key << "CircleCollider2DComponent";
      out << YAML::BeginMap; // CircleCollider2DComponent

      auto& cc2d_component = entity.GetComponent<CircleCollider2DComponent>();
      out << YAML::Key << "Offset" << YAML::Value << cc2d_component.Offset;
      out << YAML::Key << "Radius" << YAML::Value << cc2d_component.Radius;
      out << YAML::Key << "Density" << YAML::Value << cc2d_component.Density;
      out << YAML::Key << "Friction" << YAML::Value << cc2d_component.Friction;
      out << YAML::Key << "Restituition" << YAML::Value << cc2d_component.Restituition;
      out << YAML::Key << "Restituition Threshold" << YAML::Value << cc2d_component.RestituitionThreshold;

      out << YAML::EndMap; // CircleCollider2DComponent
    }

    if (entity.HasComponent<ScriptComponent>())
    {
      out << YAML::Key << "ScriptComponent";
      out << YAML::BeginMap; // ScriptComponent

      auto& script_component = entity.GetComponent<ScriptComponent>();
      out << YAML::Key << "Name" << YAML::Value << script_component.Name;

      Ref<ScriptClass> entity_class = ScriptEngine::GetEntityClass(script_component.Name);
      const auto& fields = entity_class->GetScriptFields();

      if (fields.size() > 0)
      {
        out << YAML::Key << "ScriptFields";
        out << YAML::BeginSeq;
        auto& entity_fields = ScriptEngine::GetScriptFieldMap(entity.GetUUID());
        for (const auto& [name, field] : fields)
        {
          if (entity_fields.find(name) == entity_fields.end())
            continue;

          out << YAML::BeginMap;
          out << YAML::Key << "Name" << YAML::Value << name;
          out << YAML::Key << "Type" << YAML::Value << Utilities::FieldType_ScriptFieldTypeToString(field.Type);

          out << YAML::Key << "Value" << YAML::Value;
          ScriptFieldInstance& script_field = entity_fields.at(name);
          switch (field.Type)
          {
            case ScriptFieldType::Bool:     out << script_field.GetValue<bool>(); break;
            case ScriptFieldType::Float:    out << script_field.GetValue<float>(); break;
            case ScriptFieldType::Double:   out << script_field.GetValue<double>(); break;
            case ScriptFieldType::Decimal:  out << script_field.GetValue<double>(); break;
            case ScriptFieldType::Long:     out << script_field.GetValue<int64_t>(); break;
            case ScriptFieldType::Int:      out << script_field.GetValue<int64_t>(); break;
            case ScriptFieldType::Short:    out << script_field.GetValue<int32_t>(); break;
            case ScriptFieldType::Byte:     out << script_field.GetValue<int16_t>(); break;
            case ScriptFieldType::ULong:    out << script_field.GetValue<uint64_t>(); break;
            case ScriptFieldType::UInt:     out << script_field.GetValue<uint64_t>(); break;
            case ScriptFieldType::UShort:   out << script_field.GetValue<uint32_t>(); break;
            case ScriptFieldType::UByte:    out << script_field.GetValue<uint16_t>(); break;
            case ScriptFieldType::Vector2:  out << script_field.GetValue<glm::vec2>(); break;
            case ScriptFieldType::Vector3:  out << script_field.GetValue<glm::vec3>(); break;
            case ScriptFieldType::Vector4:  out << script_field.GetValue<glm::vec3>(); break;
            case ScriptFieldType::Entity:   out << script_field.GetValue<UUID>(); break;
          }

          out << YAML::EndMap;
        }
        out << YAML::EndSeq;
      }

      out << YAML::EndMap; // ScriptComponent
    }

    out << YAML::EndMap; // Entity
  }

  void SceneSerializer::Serialize(const std::string& filepath)
  {
    YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
  }

  void SceneSerializer::SerializeRuntime(const std::string& filepath)
  {
    BS_ASSERT(false, "Not implemented yet");
  }

  bool SceneSerializer::Deserialize(const std::string& filepath)
  {
    YAML::Node data;
      
    try 
    {
      data = YAML::LoadFile(filepath);
    }
    catch (YAML::ParserException e)
    {
      BS_TRACE("Failed to load the .bshot file \"{0}\". {1}", filepath, e.what());
      return false;
    }
      
    if (!data["Scene"])
      return false;

    std::string sceneName = data["Scene"].as<std::string>();
    BS_TRACE("Deserializing scene '{0}'", sceneName);

    auto entities = data["Entities"];
    if (entities)
    {
      for (auto entity : entities)
      {
        uint64_t uuid = entity["Entity"].as<uint64_t>();

        std::string name;
        auto tagComponent = entity["TagComponent"];
        if (tagComponent)
          name = tagComponent["Tag"].as<std::string>();

        BS_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

        Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

        auto transformComponent = entity["TransformComponent"];
        if (transformComponent)
        {
          // Entities always have transforms
          auto& tc = deserializedEntity.GetComponent<TransformComponent>();
          tc.Position = transformComponent["Position"].as<glm::vec3>();
          tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
          tc.Scale = transformComponent["Scale"].as<glm::vec3>();
        }

        auto cameraComponent = entity["CameraComponent"];
        if (cameraComponent)
        {
          auto& cc = deserializedEntity.AddComponent<CameraComponent>();

          YAML::Node cameraProps = cameraComponent["Camera"];
          cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

          cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
          cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
          cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

          cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
          cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
          cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

          cc.Primary = cameraComponent["Primary"].as<bool>();
          cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
        }

        auto spriteRendererComponent = entity["SpriteRendererComponent"];
        if (spriteRendererComponent)
        {
          auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
          src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
          if (spriteRendererComponent["HasTexture"].as<bool>() == true)
          {
            src.Texture = Texture2D::Create(spriteRendererComponent["Path"].as<std::string>());
            src.TilingFactor = spriteRendererComponent["Tiling Factor"].as<float>();
          }
        }

        auto circle_renderer_component = entity["CircleRendererComponent"];
        if (circle_renderer_component)
        {
          auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
          crc.Color = circle_renderer_component["Color"].as<glm::vec4>();
          crc.Thickness = circle_renderer_component["Thickness"].as<float>();
          crc.Fade = circle_renderer_component["Fade"].as<float>();
        }

        auto rigidbody2d_component = entity["Rigidbody2DComponent"];
        if (rigidbody2d_component)
        {
          auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
          rb2d.Type = (Rigidbody2DComponent::BodyType)rigidbody2d_component["Type"].as<int>();
          rb2d.FixedRotation = rigidbody2d_component["Fixed Rotation"].as<bool>();
        }

        auto boxcollider2d_component = entity["BoxCollider2DComponent"];
        if (boxcollider2d_component)
        {
          auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
          bc2d.Size = boxcollider2d_component["Size"].as<glm::vec2>();
          bc2d.Offset = boxcollider2d_component["Offset"].as<glm::vec2>();
          bc2d.Density = boxcollider2d_component["Density"].as<float>();
          bc2d.Friction = boxcollider2d_component["Friction"].as<float>();
          bc2d.Restituition = boxcollider2d_component["Restituition"].as<float>();
          bc2d.RestituitionThreshold = boxcollider2d_component["Restituition Threshold"].as<float>();
        }

        auto circlecollider2d_component = entity["CircleCollider2DComponent"];
        if (circlecollider2d_component)
        {
          auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
          cc2d.Offset = circlecollider2d_component["Offset"].as<glm::vec2>();
          cc2d.Radius = circlecollider2d_component["Radius"].as<float>();
          cc2d.Density = circlecollider2d_component["Density"].as<float>();
          cc2d.Friction = circlecollider2d_component["Friction"].as<float>();
          cc2d.Restituition = circlecollider2d_component["Restituition"].as<float>();
          cc2d.RestituitionThreshold = circlecollider2d_component["Restituition Threshold"].as<float>();
        }

        auto script_component = entity["ScriptComponent"];
        if (script_component)
        {
          auto& script = deserializedEntity.AddComponent<ScriptComponent>();
          script.Name = script_component["Name"].as<std::string>();

          auto script_fields = script_component["ScriptFields"];
          if (script_fields)
          {
            Ref<ScriptClass> entity_class = ScriptEngine::GetEntityClass(script.Name);
            if (entity_class)
            {
              const auto& class_fields = entity_class->GetScriptFields();
              auto& entity_fields = ScriptEngine::GetScriptFieldMap(deserializedEntity.GetUUID());

              for (auto script_field : script_fields)
              {
                std::string name = script_field["Name"].as<std::string>();
                std::string typeString = script_field["Type"].as<std::string>();
                ScriptFieldType type = Utilities::FieldType_ScriptToScriptFieldType(typeString);
                ScriptFieldInstance& field_instance = entity_fields[name];

                // Make it an error instead of assert
                BS_ASSERT(class_fields.find(name) != class_fields.end(), "Unexpected Error");

                if (class_fields.find(name) == class_fields.end())
                  continue;

                field_instance.Field = class_fields.at(name);

                switch (type)
                {
                case ScriptFieldType::Bool:
                {
                  bool value = script_field["Value"].as<bool>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Float:
                {
                  float value = script_field["Value"].as<float>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Double:
                {
                  double value = script_field["Value"].as<double>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Decimal:
                {
                  double value = script_field["Value"].as<double>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Long:
                {
                  int64_t value = script_field["Value"].as<int64_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Int:
                {
                  int32_t value = script_field["Value"].as<int32_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Short:
                {
                  int16_t value = script_field["Value"].as<int16_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Byte:
                {
                  int8_t value = script_field["Value"].as<int8_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::ULong:
                {
                  uint64_t value = script_field["Value"].as<uint64_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::UInt:
                {
                  uint32_t value = script_field["Value"].as<uint32_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::UShort:
                {
                  uint16_t value = script_field["Value"].as<uint16_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::UByte:
                {
                  uint8_t value = script_field["Value"].as<uint8_t>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Vector2:
                {
                  glm::vec2 value = script_field["Value"].as<glm::vec2>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Vector3:
                {
                  glm::vec3 value = script_field["Value"].as<glm::vec3>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Vector4:
                {
                  glm::vec4 value = script_field["Value"].as<glm::vec4>();
                  field_instance.SetValue(value);
                  break;
                }
                case ScriptFieldType::Entity:
                {
                  UUID value = script_field["Value"].as<UUID>();
                  field_instance.SetValue(value);
                  break;
                }
                }
              }
            }
          }
        }
      }
    }
    return true;
  }

  bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
  {
    BS_ASSERT(false, "Not implemented yet");
    return false;
  }

}