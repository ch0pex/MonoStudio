#pragma once

#include <rflect/window/window.hpp>

namespace vix {

template<typename SurfaceType>
struct Viewport {
  rflect::Window window {};
  SurfaceType surface {};
};

} // namespace vix
