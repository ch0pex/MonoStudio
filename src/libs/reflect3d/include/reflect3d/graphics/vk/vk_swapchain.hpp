#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/graphics/vk/vk_swapchain_detail.hpp"


namespace rf3d::gfx::vk {

class Swapchain {
public:
  using config_type  = core::SwapchainCreateInfoKHR;
  using native_type  = raii::SwapchainKHR;
  using images_type  = Image;
  using surface_type = raii::SurfaceKHR;
  using format_type  = core::Format;

  explicit Swapchain(raii::Device const& device, surface_type&& window_surface, config_type const& config) :
    render_surface(std::move(window_surface)), //
    native_handle(device, config), //
    chain_images(detail::get_images(device, native_handle, config.imageFormat)) { }


private:
  surface_type render_surface;
  native_type native_handle;
  std::vector<images_type> chain_images;
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
