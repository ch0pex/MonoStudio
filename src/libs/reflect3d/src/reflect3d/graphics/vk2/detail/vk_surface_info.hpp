#pragma once

#include "reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp"

namespace rf3d::vk::detail {

struct SurfaceInfo {
  core::SurfaceCapabilitiesKHR capabilities;
  std::vector<core::SurfaceFormatKHR> formats;
  std::vector<core::PresentModeKHR> present_modes;
};

} // namespace rf3d::vk::detail
