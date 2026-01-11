#pragma once

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <stdexcept>
#endif

namespace rf3d::gfx::vk {

struct Exception : std::runtime_error {
  using runtime_error::runtime_error;
};


} // namespace rf3d::gfx::vk
