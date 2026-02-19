#pragma once


#include "reflect3d/graphics/vk/vk_core.hpp"

namespace rf3d::gfx::vk {

struct Traits {
  using renderer = ::rf3d::gfx::vk::Core;
  using surface  = ::rf3d::gfx::vk::Surface;
};

} // namespace rf3d::gfx::vk
