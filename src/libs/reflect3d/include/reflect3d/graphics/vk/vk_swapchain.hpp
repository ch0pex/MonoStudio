#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/graphics/vk/vk_swapchain_detail.hpp"


namespace rf3d::gfx::vk {

struct Semaphores {
  Semaphores(raii::Device const& device) :
    present(device, core::SemaphoreCreateInfo {}), //
    render(device, core::SemaphoreCreateInfo {}) //
  { }

  raii::Semaphore present;
  raii::Semaphore render;
};


class Swapchain {
public:
  using config_type    = core::SwapchainCreateInfoKHR;
  using native_type    = raii::SwapchainKHR;
  using images_type    = Image;
  using surface_type   = raii::SurfaceKHR;
  using format_type    = core::Format;
  using extent_type    = core::Extent2D;
  using semaphore_type = raii::Semaphore;


  explicit Swapchain(raii::Device const& device, surface_type&& window_surface, config_type const& config) :
    extent_info(config.imageExtent), //
    surface(std::move(window_surface)), //
    handle(device, config), //
    chain_images(detail::get_images(device, handle, config.imageFormat)) //
  {
    for (std::size_t i = 0; i < chain_images.size(); ++i) {
      semaphores.emplace_back(device);
    }
  }

  [[nodiscard]] std::tuple<std::uint32_t, Image const&> next_image(FrameIndex const frame_index) {
    auto [result, index] = handle.acquireNextImage(defaults::wait_timeout, present_semaphore(frame_index), nullptr);
    result >> check::error;
    return {index, chain_images.at(index)};
  }

  [[nodiscard]] extent_type const& extent() const noexcept { return extent_info; }

  [[nodiscard]] semaphore_type const& present_semaphore(FrameIndex const index) const noexcept {
    return semaphores.at(index).present;
  }

  [[nodiscard]] semaphore_type const& render_semaphore(FrameIndex const index) const noexcept {
    return semaphores.at(index).render;
  }

  native_type const& operator*() const noexcept { return handle; }

private:
  extent_type extent_info;
  surface_type surface;
  native_type handle;
  std::vector<images_type> chain_images;
  std::vector<Semaphores> semaphores;
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
