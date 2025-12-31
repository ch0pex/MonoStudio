#pragma once

#include <stdexcept>

namespace rf3d::gfx::vk {

struct Exception : std::runtime_error {
  using runtime_error::runtime_error;
};


} // namespace rf3d::gfx::vk
