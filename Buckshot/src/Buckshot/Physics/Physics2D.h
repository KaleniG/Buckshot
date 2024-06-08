#pragma once

#include <box2d/b2_body.h>

#include "Buckshot/Core/Base.h"
#include "Buckshot/Scene/Components.h"

namespace Buckshot {
  namespace Utilities {

    inline b2BodyType Rigidbody2D_BuckshotToBox2D(Rigidbody2DComponent::BodyType type)
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

    inline Rigidbody2DComponent::BodyType Rigidbody2D_Box2DToBuckshot(b2BodyType type)
    {
      switch (type)
      {
      case b2_dynamicBody:
        return Rigidbody2DComponent::BodyType::Dynamic;
      case b2_staticBody:
        return Rigidbody2DComponent::BodyType::Static;
      case b2_kinematicBody:
        return Rigidbody2DComponent::BodyType::Kinematic;
      }

      BS_ASSERT(false, "Unknown BodyType");
      return Rigidbody2DComponent::BodyType::Static;
    }

  }
}