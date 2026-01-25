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

  Swapchain(config_type const& config) :
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

  Resolution resolution() const noexcept {
    return Resolution {
      .width  = static_cast<std::uint16_t>(extent.width),
      .height = static_cast<std::uint16_t>(extent.height),
    };
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

class Surface {
public:
  using images_type  = Image;
  using surface_type = raii::SurfaceKHR;
  using format_type  = core::Format;
  using window_type  = Window;

  explicit Surface(Window&& window) :
    window_handle(std::move(window)), //
    window_surface(instance::create_surface(window_handle.native_handle())), //
    swapchain(create_swapchain_config(
        window_surface, //
        gpu::get_surface_info(window_surface), //
        window_handle.resolution() //
    )) { }

  [[nodiscard]] std::optional<ImageProxy> next_image(FrameIndex const frame_index) {

    auto image = swapchain.next_image(frame_index);
    if (not image) {
      // recreate swapchain
      return std::nullopt;
    }
    return image;
  }

  [[nodiscard]] Swapchain::semaphore_type const& present_semaphore(FrameIndex const index) const noexcept {
    return swapchain.present_semaphore(index);
  }

  [[nodiscard]] Swapchain::semaphore_type const& render_semaphore() const noexcept {
    return swapchain.render_semaphore();
  }

  [[nodiscard]] Resolution winwdow_size() const {
    auto res = window_handle.resolution();
    return res;
  }

  [[nodiscard]] Resolution resolution() const {
    auto res = swapchain.resolution();
    return res;
  }

  void present() {
    gpu::present(swapchain.present_info()).or_else([](auto&& err) { // NOLINT
      LOG_INFO("Recreating swapchain due to: {}", err.what());
      // gpu.recreate_swapchain(swapchain);
      return mono::err::expected<void> {};
    });
  }

private:
  window_type window_handle;
  surface_type window_surface;
  Swapchain swapchain;
};


} // namespace rf3d::gfx::vk
