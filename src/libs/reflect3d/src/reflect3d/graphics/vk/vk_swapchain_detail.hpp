#pragma once

#include <vulkan/vulkan.hpp>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
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
choose_swap_extent(Resolution const resolution, core::SurfaceCapabilitiesKHR const& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  return {
    .width = std::clamp<uint32_t>(
        resolution.width, //
        capabilities.minImageExtent.width, //
        capabilities.maxImageExtent.width
    ),
    .height = std::clamp<uint32_t>(
        resolution.height, //
        capabilities.minImageExtent.height, //
        capabilities.maxImageExtent.height
    )
  };
}

inline std::vector<Image> get_images(
    raii::SwapchainKHR const& swapchain, //
    core::Format const format //
) {
  core::ImageViewCreateInfo view_info {
    .viewType         = core::ImageViewType::e2D,
    .format           = format,
    .subresourceRange = {
      .aspectMask     = core::ImageAspectFlagBits::eColor,
      .baseMipLevel   = 0,
      .levelCount     = 1,
      .baseArrayLayer = 0,
      .layerCount     = 1
    }
  };

  auto build_image = [&](core::Image const& img) { //
    view_info.image = img;
    return Image {img, gpu::make_image_view(view_info)};
  };

  return swapchain.getImages() //
         | std::views::transform(build_image) //
         | std::ranges::to<std::vector>();
}

} // namespace rf3d::gfx::vk::detail
