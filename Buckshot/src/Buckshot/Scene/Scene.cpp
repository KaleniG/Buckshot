#include <bspch.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <glm/glm.hpp>
#include <entt.hpp>

#include "Buckshot/Renderer/Renderer2D.h"
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
    Entity entity = { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = (name.empty()) ? "Entity" : name;
    return entity;
  }

  Buckshot::Entity Scene::DuplicateEntity(Entity& other)
  {
    Entity entity = { m_Registry.create(), this };

    if (other.HasComponent<TagComponent>())
    {
      auto& tag = entity.AddComponent<TagComponent>();
      auto& tag_other = other.GetComponent<TagComponent>();
      std::string new_name = tag_other.Tag;
      new_name.append(" (Copy)");
      tag.Tag = new_name;
    }

    if (other.HasComponent<TransformComponent>())
    {
      auto& transform = entity.AddComponent<TransformComponent>();
      auto& transform_other = other.GetComponent<TransformComponent>();
      transform.Position = transform_other.Position;
      transform.Rotation = transform_other.Rotation;
      transform.Scale = transform_other.Scale;
    }

    if (other.HasComponent<SpriteRendererComponent>())
    {
      auto& src = entity.AddComponent<SpriteRendererComponent>();
      auto& src_other = other.GetComponent<SpriteRendererComponent>();
      src.Color = src_other.Color;
      src.Texture = src_other.Texture;
      src.TilingFactor = src_other.TilingFactor;
    }

    if (other.HasComponent<CameraComponent>())
    {
      auto& camera = entity.AddComponent<CameraComponent>();
      auto& camera_other = other.GetComponent<CameraComponent>();
      camera.Camera = camera_other.Camera;
      camera.FixedAspectRatio = camera_other.FixedAspectRatio;
      camera.Primary = false;
    }

    if (other.HasComponent<Rigidbody2DComponent>())
    {
      auto& rb2d = entity.AddComponent<Rigidbody2DComponent>();
      auto& rb2d_other = other.GetComponent<Rigidbody2DComponent>();
      rb2d.Type = rb2d_other.Type;
      rb2d.FixedRotation = rb2d_other.FixedRotation;
    }

    if (other.HasComponent<BoxCollider2DComponent>())
    {
      auto& bc2d = entity.AddComponent<BoxCollider2DComponent>();
      auto& bc2d_other = other.GetComponent<BoxCollider2DComponent>();
      bc2d.Size = bc2d_other.Size;
      bc2d.Offset = bc2d_other.Offset;
      bc2d.Density = bc2d_other.Density;
      bc2d.Friction = bc2d_other.Friction;
      bc2d.Restituition = bc2d_other.Restituition;
      bc2d.RestituitionThreshold = bc2d_other.RestituitionThreshold;
    }

    return entity;
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

    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group)
    {
      auto transform = group.get<TransformComponent>(entity);
      auto sprite = group.get<SpriteRendererComponent>(entity);

      Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
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

      auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
      for (auto entity : group)
      {
        auto transform = group.get<TransformComponent>(entity);
        auto sprite = group.get<SpriteRendererComponent>(entity);

        Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
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