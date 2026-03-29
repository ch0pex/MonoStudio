#pragma once

#include "reflect3d/math/math.hpp"

namespace rf3d {

struct Rect2D {
  math::uvec2 offset;
  math::uvec2 extent;
};

struct Viewport {
  Rect2D rect;
  float min_depth;
  float max_depth;
};

} // namespace rf3d
