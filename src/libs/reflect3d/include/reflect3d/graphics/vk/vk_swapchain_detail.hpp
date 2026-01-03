#pragma once

#include <vulkan/vulkan.hpp>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_surface_info.hpp"
#include "reflect3d/window/utils/resolution.hpp"

namespace rf3d::gfx::vk::detail {

inline uint32_t choose_swap_min_image_count(core::SurfaceCapabilitiesKHR const& surfaceCapabilities) {
  auto minImageCount = std::max(3U, surfaceCapabilities.minImageCount);
  if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)) {
    minImageCount = surfaceCapabilities.maxImageCount;
  }
  return minImageCount;
}

inline core::SurfaceFormatKHR choose_surface_format(std::vector<core::SurfaceFormatKHR> const& availableFormats) {
  assert(!availableFormats.empty());
  auto const formatIt = std::ranges::find_if(availableFormats, [](auto const& format) {
    return format.format == core::Format::eB8G8R8A8Srgb && format.colorSpace == core::ColorSpaceKHR::eSrgbNonlinear;
  });
  return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
}

inline core::PresentModeKHR choose_present_mode(std::vector<core::PresentModeKHR> const& availablePresentModes) {
  assert(std::ranges::any_of(availablePresentModes, [](auto presentMode) {
    return presentMode == core::PresentModeKHR::eFifo;
  }));

  bool const support_mailbox = std::ranges::any_of(availablePresentModes, [](core::PresentModeKHR const value) {
    return core::PresentModeKHR::eMailbox == value;
  });

  return support_mailbox ? core::PresentModeKHR::eMailbox : core::PresentModeKHR::eFifo;
}

inline core::Extent2D
choose_swap_extent(Resolution const& resolution, core::SurfaceCapabilitiesKHR const& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  return {
    .width =
        std::clamp<uint32_t>(resolution.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
    .height =
        std::clamp<uint32_t>(resolution.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
  };
}


} // namespace rf3d::gfx::vk::detail
