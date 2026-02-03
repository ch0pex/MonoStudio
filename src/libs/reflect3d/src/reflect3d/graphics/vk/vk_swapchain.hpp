#pragma once

#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/graphics/vk/vk_swapchain_detail.hpp"

#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/containers/stable_vector.hpp>

namespace rf3d::gfx::vk {

struct Semaphores {
  raii::Semaphore present {gpu::make_semaphore()};
  raii::Semaphore render {gpu::make_semaphore()};
};

class Swapchain {
public:
  using config_type    = core::SwapchainCreateInfoKHR;
  using extent_type    = core::Extent2D;
  using native_type    = raii::SwapchainKHR;
  using image_type     = Image;
  using semaphore_type = raii::Semaphore;

  explicit Swapchain(config_type const& config) :
    handle(gpu::make_swapchain(config)), //
    extent(config.imageExtent), //
    images(detail::get_images(handle, config.imageFormat)) //
  {
    semaphores.resize(images.size());
  }

  Swapchain(Swapchain const&) = delete;

  Swapchain& operator=(Swapchain const&) = delete;

  Swapchain(Swapchain&&) = default;

  Swapchain& operator=(Swapchain&&) = default;

  ~Swapchain() { //
    if (handle != nullptr) {
      gpu::wait_idle();
      LOG_INFO("Destroying Swapchain");
    }
  }

  [[nodiscard]] std::optional<ImageProxy> next_image(FrameIndex const frame_index) try {
    auto [result, index] = handle.acquireNextImage(defaults::wait_timeout, *present_semaphore(frame_index), nullptr);
    image_index          = index;
    return images.at(index);
  }
  catch (std::exception const& e) {
    return std::nullopt;
  }

  [[nodiscard]] semaphore_type const& present_semaphore(FrameIndex const index) const noexcept {
    return semaphores.at(index % defaults::max_frames_in_flight).present;
  }

  [[nodiscard]] semaphore_type const& render_semaphore() const noexcept { return semaphores.at(image_index).render; }

  [[nodiscard]] core::PresentInfoKHR present_info() const {
    return core::PresentInfoKHR {
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = std::addressof(*render_semaphore()),
      .swapchainCount     = 1,
      .pSwapchains        = std::addressof(*handle),
      .pImageIndices      = std::addressof(image_index),
    };
  }

  [[nodiscard]] Resolution resolution() const noexcept {
    return Resolution {
      .width  = static_cast<std::uint16_t>(extent.width),
      .height = static_cast<std::uint16_t>(extent.height),
    };
  }

  void recreate(config_type config) {
    config.oldSwapchain    = *handle;
    native_type new_handle = gpu::make_swapchain(config);
    handle                 = std::move(new_handle);
    extent                 = config.imageExtent;
    images                 = detail::get_images(handle, config.imageFormat);
    image_index            = 0;
  }

private:
  native_type handle;
  extent_type extent;
  std::vector<image_type> images;
  std::vector<Semaphores> semaphores;
  ImageIndex image_index {0};
};

/*
 * Helper function to create a swapchain configuration
 * based on the provided surface details and resolution.
 */
inline Swapchain::config_type create_swapchain_config(
    raii::SurfaceKHR const& surface, //
    SurfaceInfo const& surface_details, //
    Resolution const& resolution //
) {
  auto const [format, colorSpace] = detail::choose_surface_format(surface_details.formats);
  auto const present_mode         = detail::choose_present_mode(surface_details.present_modes);

  return {
    .surface          = *surface,
    .minImageCount    = detail::choose_swap_min_image_count(surface_details.capabilities),
    .imageFormat      = format,
    .imageColorSpace  = colorSpace,
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
