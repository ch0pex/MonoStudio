#pragma once

#include "reflect3d/math/math.hpp"

namespace rf3d::gfx {

struct Vertex {
  math::vec3 position {0.0F, 0.0F, 0.0F};
  math::vec4 color {0.4F, 0.4F, 0.4F, 0.0F};
  // math::vec3 normal {0.0F, 0.0F, 0.0F};
  // math::vec2 uv {0.0F, 0.0F};
};

} // namespace rf3d::gfx
