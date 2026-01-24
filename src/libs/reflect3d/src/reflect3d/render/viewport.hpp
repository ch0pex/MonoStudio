#pragma once

#include "reflect3d/window/window.hpp"

namespace vix {

template<typename SurfaceType>
struct Viewport {
  rf3d::Window window;
  SurfaceType surface;
};

} // namespace vix
