#pragma once

#include "reflect3d/math/math.hpp"

namespace rf3d {

struct Rect2D {
  math::uvec2 offset {0, 0};
  math::uvec2 extent {0, 0};
};

struct Viewport {
  Rect2D rect {};
  float min_depth {0.0F};
  float max_depth {1.0F};
};

} // namespace rf3d
