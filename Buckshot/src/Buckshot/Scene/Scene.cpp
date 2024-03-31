#include <bspch.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <glm/glm.hpp>
#include <entt.hpp>

#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Scene/ScriptableEntity.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scene/Scene.h"

namespace Buckshot {

  namespace Utilities {

    static b2BodyType Rigidbody2D_BuckshotToBox2D(Rigidbody2DComponent::BodyType type)
    {
      switch (type)
      {
        case Rigidbody2DComponent::BodyType::Dynamic:
          return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Static:
          return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Kinematic:
          return b2_kinematicBody;
      }

      BS_ASSERT(false, "Unknown BodyType");
      return b2_staticBody;
    }

    template<typename Component>
    static void CopyComponent(entt::registry& destination, entt::registry& source, const std::unordered_map<UUID, entt::entity>& entt_map)
    {
      auto source_view = source.view<Component>();
      for (auto e : source_view)
      {
        entt::entity destination_entity_id = entt_map.at(source.get<IDComponent>(e).ID);

        auto& component = source.get<Component>(e);
        destination.emplace_or_replace<Component>(destination_entity_id, component);
      }
    }

    template<typename Component>
    static void CopyComponentIfExists(Entity destination, Entity source)
    {
      if (source.HasComponent<Component>())
      {
        destination.AddOrReplaceComponent<Component>(source.GetComponent<Component>());
      }
    }

  }

  Ref<Scene> Scene::Copy(Ref<Scene> other)
  {
    Ref<Scene> new_scene = CreateRef<Scene>();

    new_scene->m_ViewportWidth = other->m_ViewportWidth;
    new_scene->m_ViewportHeight = other->m_ViewportHeight;

    auto& other_registry = other->m_Registry;
    auto& new_registry = new_scene->m_Registry;

    std::unordered_map<UUID, entt::entity> entt_map;

    auto id_view = other_registry.view<IDComponent>();
    for (auto e : id_view)
    {
      UUID uuid = other_registry.get<IDComponent>(e).ID;
      const auto& name = other_registry.get<TagComponent>(e).Tag;
      Entity new_entity = new_scene->CreateEntityWithUUID(uuid, name);
      entt_map[uuid] = (entt::entity)new_entity;
    }

    Utilities::CopyComponent<TransformComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<SpriteRendererComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<CircleRendererComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<CameraComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<NativeScriptComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<Rigidbody2DComponent>(new_registry, other_registry, entt_map);
    Utilities::CopyComponent<BoxCollider2DComponent>(new_registry, other_registry, entt_map);

    return new_scene;
  }

  Scene::Scene()
  {

  } 

  Scene::~Scene()
  {
    delete m_PhysicsWorld;
  }

  Entity Scene::CreateEntity(const std::string& name)
  {
    return CreateEntityWithUUID(UUID(), name);
  }

  Buckshot::Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
  {
    Entity entity = { m_Registry.create(), this };

    entity.AddComponent<IDComponent>(uuid);

    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = (name.empty()) ? "Entity" : name;

    entity.AddComponent<TransformComponent>();

    return entity;
  }

  Buckshot::Entity Scene::DuplicateEntity(Entity& other_entity)
  {
    std::string new_name = other_entity.GetName();
    new_name.append(" (Copy)");

    Entity new_entity = CreateEntity(new_name);

    Utilities::CopyComponentIfExists<TransformComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<SpriteRendererComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<CircleRendererComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<CameraComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<NativeScriptComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<Rigidbody2DComponent>(new_entity, other_entity);
    Utilities::CopyComponentIfExists<BoxCollider2DComponent>(new_entity, other_entity);

    return new_entity;
  }

  void Scene::DestroyEntity(Entity entity)
  {
    m_Registry.destroy(entity);
  }

  void Scene::OnRuntimeStart()
  {
    m_PhysicsWorld = new b2World(b2Vec2(0.0f, -9.8f));
    auto view = m_Registry.view<Rigidbody2DComponent>();
    for (auto e : view)
    {
      Entity entity(e, this);

      auto& transform_component = entity.GetComponent<TransformComponent>();
      auto& rb2d_component = entity.GetComponent<Rigidbody2DComponent>();

      b2BodyDef body_definition;
      body_definition.type = Utilities::Rigidbody2D_BuckshotToBox2D(rb2d_component.Type);
      body_definition.position.Set(transform_component.Position.x, transform_component.Position.y);
      body_definition.angle = transform_component.Rotation.z;

      b2Body* body = m_PhysicsWorld->CreateBody(&body_definition);
      body->SetFixedRotation(rb2d_component.FixedRotation);
      rb2d_component.RuntimeBody = body;

      if (entity.HasComponent<BoxCollider2DComponent>())
      {
        auto& bc2d_component = entity.GetComponent<BoxCollider2DComponent>();

        b2PolygonShape box_shape;
        box_shape.SetAsBox(bc2d_component.Size.x * transform_component.Scale.x, bc2d_component.Size.y * transform_component.Scale.y);

        b2FixtureDef fixture_definition;
        fixture_definition.shape = &box_shape;
        fixture_definition.density = bc2d_component.Density;
        fixture_definition.friction = bc2d_component.Friction;
        fixture_definition.restitution = bc2d_component.Restituition;
        fixture_definition.restitutionThreshold = bc2d_component.RestituitionThreshold;

        body->CreateFixture(&fixture_definition);
      }
    }
  }

  void Scene::OnRuntimeStop()
  {
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
  }

  void Scene::OnUpdateEditor(Timestep timestep, EditorCamera& camera)
  {
    Renderer2D::BeginScene(camera);

    {
      auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
      for (auto entity : group)
      {
        auto transform = group.get<TransformComponent>(entity);
        auto sprite = group.get<SpriteRendererComponent>(entity);

        Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
      }
    }

    {
      auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
      for (auto entity : view)
      {
        auto transform = view.get<TransformComponent>(entity);
        auto circle = view.get<CircleRendererComponent>(entity);

        Renderer2D::DrawCircle(transform.GetTransform(), circle, (int)entity);
      }
    }

    Renderer2D::EndScene();
  }

  void Scene::OnUpdateRuntime(Timestep timestep)
  {
    // Scripts Update
    {
      m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
          if (!nsc.Instance)
          {
            nsc.Instance = nsc.InstanciateScript();
            nsc.Instance->m_Entity = Entity(entity, this);
            nsc.Instance->OnCreate();
          }
          nsc.Instance->OnUpdate(timestep);
        }
      );
    }

    // Physics
    {
      const int32_t velocity_iterations = 6;
      const int32_t position_iterations = 2;
      m_PhysicsWorld->Step(timestep, velocity_iterations, position_iterations);

      auto view = m_Registry.view<Rigidbody2DComponent>();
      for (auto e : view)
      {
        Entity entity(e, this);

        auto& transform_component = entity.GetComponent<TransformComponent>();
        auto& rb2d_component = entity.GetComponent<Rigidbody2DComponent>();

        b2Body* body = (b2Body*)rb2d_component.RuntimeBody;
        const auto& position = body->GetPosition();
        transform_component.Position.x = position.x;
        transform_component.Position.y = position.y;
        const auto& rotation = body->GetAngle();
        transform_component.Rotation.z = rotation;

      }
    }

    // Renderer 2D Update
    Camera* main_camera = nullptr;
    glm::mat4 camera_transform;

    auto view = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view)
    {
      auto transform = view.get<TransformComponent>(entity);
      auto camera = view.get<CameraComponent>(entity);

      if (camera.Primary)
      {
        main_camera = &camera.Camera;
        camera_transform = transform.GetTransform();
        break;
      }
    }

    if (main_camera)
    {
      Renderer2D::BeginScene(main_camera->GetProjection(), camera_transform);


      // Draw Sprites
      {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
          auto transform = group.get<TransformComponent>(entity);
          auto sprite = group.get<SpriteRendererComponent>(entity);

          Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
        }
      }

      // Draw Circles
      {
        auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
        for (auto entity : view)
        {
          auto transform = view.get<TransformComponent>(entity);
          auto circle = view.get<CircleRendererComponent>(entity);

          Renderer2D::DrawCircle(transform.GetTransform(), circle, (int)entity);
        }
      }

      Renderer2D::EndScene();
    }
  }

  void Scene::OnViewportResize(uint32_t width, uint32_t height)
  {
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& cameraComponent = view.get<CameraComponent>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.SetViewportSize(width, height);
    }
  }

  void Scene::OnViewportResize()
  {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& cameraComponent = view.get<CameraComponent>(entity);
      if (!cameraComponent.FixedAspectRatio)
        cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
  }

  Entity Scene::GetPrimaryCameraEntity()
  {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
      const auto& camera = view.get<CameraComponent>(entity);
      if (camera.Primary)
        return Entity(entity, this);
    }
    return Entity();
  }

}