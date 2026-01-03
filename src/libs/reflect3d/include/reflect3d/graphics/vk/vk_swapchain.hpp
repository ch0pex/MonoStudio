#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_swapchain_detail.hpp"

namespace rf3d::gfx::vk {

class Swapchain {
public:
  using config_type  = core::SwapchainCreateInfoKHR;
  using native_type  = raii::SwapchainKHR;
  using images_type  = core::Image;
  using surface_type = raii::SurfaceKHR;

  explicit Swapchain(raii::Device const& device, surface_type&& window_surface, config_type const& config) :
    surface(std::move(window_surface)), handle(device, config), images(handle.getImages()) { }

private:
  surface_type surface;
  native_type handle;
  std::vector<images_type> images;
};

/**
 * Helper function to create a swapchain configuration
 * based on the provided surface details and resolution.
 */
inline Swapchain::config_type create_swapchain_config(
    raii::SurfaceKHR const& surface, //
    SurfaceInfo const& surface_details, //
    Resolution const& resolution //
) {
  auto const surface_format = detail::choose_surface_format(surface_details.formats);
  auto const present_mode   = detail::choose_present_mode(surface_details.present_modes);

  return {
    .surface          = *surface,
    .minImageCount    = detail::choose_swap_min_image_count(surface_details.capabilities),
    .imageFormat      = surface_format.format,
    .imageColorSpace  = surface_format.colorSpace,
    .imageExtent      = detail::choose_swap_extent(resolution, surface_details.capabilities),
    .imageArrayLayers = 1,
    .imageUsage       = core::ImageUsageFlagBits::eColorAttachment,
    .imageSharingMode = core::SharingMode::eExclusive,
    .preTransform     = surface_details.capabilities.currentTransform,
    .compositeAlpha   = core::CompositeAlphaFlagBitsKHR::eOpaque,
    .presentMode      = present_mode,
    .clipped          = core::True,
  };
}

} // namespace rf3d::gfx::vk
