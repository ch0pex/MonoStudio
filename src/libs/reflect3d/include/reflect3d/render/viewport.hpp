#pragma once

#include <rf3d/window/window.hpp>

namespace vix {

template<typename SurfaceType>
struct Viewport {
  rf3d::Window window {};
  SurfaceType surface {};
};

} // namespace vix
