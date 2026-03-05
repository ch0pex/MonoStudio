#pragma once

#include "reflect3d/math/math.hpp"

namespace rf3d::gfx {

struct Rect2D {
  math::vec2 offset;
  math::vec2 extent;
};

struct Viewport {
  Rect2D rect;
  float min_depth;
  float max_depth;
};

} // namespace rf3d::gfx
