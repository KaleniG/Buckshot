#include <bspch.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
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
#include "Buckshot/Scripting/ScriptEngine.h"
#include "Buckshot/Physics/Physics2D.h"

namespace Buckshot {

  namespace Utilities {

    template<typename... Component>
    static void CopyComponent(entt::registry& destination, entt::registry& source, const std::unordered_map<UUID, entt::entity>& entt_map)
    {
      ([&]() 
      {
        auto view = source.view<Component>();
        for (auto source_entity : view)
        {
          entt::entity destination_entity = entt_map.at(source.get<IDComponent>(source_entity).ID);
          auto& source_component = source.get<Component>(source_entity);
          destination.emplace_or_replace<Component>(destination_entity, source_component);
        }
      }(), ...);
    }

    template<typename... Component>
    static void CopyComponent(ComponentGroup<Component...>, entt::registry& destination, entt::registry& source, const std::unordered_map<UUID, entt::entity>& entt_map)
    {
      CopyComponent<Component...>(destination, source, entt_map);
    }

    template<typename... Component>
    static void CopyComponentIfExists(Entity destination, Entity source)
    {
      ([&]()
      {
        if (source.HasComponent<Component>())
          destination.AddOrReplaceComponent<Component>(source.GetComponent<Component>());
      }(), ...);
    }

    template<typename... Component>
    static void CopyComponentIfExists(ComponentGroup<Component...>, Entity destination, Entity source)
    {
      CopyComponentIfExists<Component...>(destination, source);
    }

  }

  Ref<Scene> Scene::Copy(Ref<Scene> other)
  {
    Ref<Scene> new_scene = CreateRef<Scene>();

    new_scene->m_ViewportWidth = other->m_ViewportWidth;
    new_scene->m_ViewportHeight = other->m_ViewportHeight;

    new_scene->m_IsRunning = other->m_IsRunning;

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

    Utilities::CopyComponent(AllComponents{}, new_registry, other_registry, entt_map);

    return new_scene;
  }

  void Scene::OnPhysics2DStart()
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

      if (entity.HasComponent<CircleCollider2DComponent>())
      {
        auto& cc2d_component = entity.GetComponent<CircleCollider2DComponent>();

        b2CircleShape circle_shape;
        circle_shape.m_p.Set(cc2d_component.Offset.x, cc2d_component.Offset.y);
        circle_shape.m_radius = cc2d_component.Radius * transform_component.Scale.x;

        b2FixtureDef fixture_definition;
        fixture_definition.shape = &circle_shape;
        fixture_definition.density = cc2d_component.Density;
        fixture_definition.friction = cc2d_component.Friction;
        fixture_definition.restitution = cc2d_component.Restituition;
        fixture_definition.restitutionThreshold = cc2d_component.RestituitionThreshold;

        body->CreateFixture(&fixture_definition);
      }
    }
  }

  void Scene::OnPhysics2DStop()
  {
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
  }

  void Scene::RenderScene(EditorCamera& camera)
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

    m_EntityMap[uuid] = entity;

    return entity;
  }

  Buckshot::Entity Scene::DuplicateEntity(Entity& other_entity)
  {
    std::string new_name = other_entity.GetName();
    new_name.append(" (Copy)");

    Entity new_entity = CreateEntity(new_name);

    Utilities::CopyComponentIfExists(AllComponents{}, new_entity, other_entity);

    m_EntityMap[new_entity.GetUUID()] = new_entity;

    return new_entity;
  }

  void Scene::DestroyEntity(Entity entity)
  {
    m_EntityMap.erase(entity.GetUUID());
    m_Registry.destroy(entity);
  }

  void Scene::OnRuntimeStart()
  {
    m_IsRunning = true;

    // Physics
    {
      OnPhysics2DStart();
    }

    // Scripts
    {
      ScriptEngine::OnRuntimeStart(this);

      auto view = m_Registry.view<ScriptComponent>();
      for (auto e : view)
      {
        Entity entity = Entity(e, this);
        ScriptEngine::OnCreateEntity(entity);
      }
    }
  }

  void Scene::OnRuntimeStop()
  {
    m_IsRunning = false;
    OnPhysics2DStop();
    ScriptEngine::OnRuntimeStop();
  }

  void Scene::OnSimulationStart()
  {
    OnPhysics2DStart();
  }

  void Scene::OnSimulationStop()
  {
    OnPhysics2DStop();
  }

  void Scene::OnUpdateEditor(Timestep timestep, EditorCamera& camera)
  {
    RenderScene(camera);
  }

  void Scene::OnUpdateRuntime(Timestep timestep)
  {
    if (!m_IsPaused || m_StepFrames-- > 0)
    {
      // C# Scripts Update
      {
        auto view = m_Registry.view<ScriptComponent>();
        for (auto e : view)
        {
          Entity entity = Entity(e, this);
          ScriptEngine::OnUpdateEntity(entity, timestep);
        }
      }

      // Native Scripts Update
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

  void Scene::OnUpdateSimulation(Timestep timestep, EditorCamera& camera)
  {
    if (!m_IsPaused || m_StepFrames-- > 0)
    {
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
    }

    RenderScene(camera);
  }

  void Scene::OnViewportResize(uint32_t width, uint32_t height)
  {
    if (m_ViewportWidth == width && m_ViewportHeight == height)
      return;

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

  Entity Scene::GetEntityByUUID(UUID uuid)
  {
    if (m_EntityMap.find(uuid) != m_EntityMap.end())
      return Entity(m_EntityMap.at(uuid), this);

    return Entity();
  }

  Entity Scene::GetEntityByName(const std::string& name)
  {
    auto view = m_Registry.view<TagComponent>();
    for (auto entity : view)
    {
      auto& tag_component = view.get<TagComponent>(entity);
      if (tag_component.Tag == name)
        return Entity(entity, this);
    }
    return Entity();
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

  void Scene::Step(int frames)
  {
    m_StepFrames = frames;
  }

}