#pragma once

#include <glm/glm.hpp>

namespace Buckshot::Math {

  bool DecomposeTransform(const glm::mat4& transform, glm::vec3& out_position, glm::vec3& out_rotation, glm::vec3& out_scale);

}