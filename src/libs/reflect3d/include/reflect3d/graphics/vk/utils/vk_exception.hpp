#pragma once

#include <stdexcept>

namespace rf3d::hri::vk {

struct Exception : std::runtime_error {
  using runtime_error::runtime_error;
};


} // namespace rf3d::hri::vk
