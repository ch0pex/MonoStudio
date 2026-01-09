#pragma once

#include <chrono>
#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/graphics/vk/vk_swapchain_detail.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <mono/containers/stable_vector.hpp>

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

  ~Swapchain() { //
    if (handle != nullptr) {
      LOG_INFO("Destroying Swapchain");
    }
  }

  Swapchain(Swapchain const&) = delete;

  Swapchain(Swapchain&&) = default;

  Swapchain& operator=(Swapchain const&) = delete;

  Swapchain& operator=(Swapchain&&) = default;

  [[nodiscard]] Image const& next_image(FrameIndex const frame_index) {
    auto [result, index] = handle.acquireNextImage(defaults::wait_timeout, present_semaphore(frame_index), nullptr);

    result >> check::error;
    image_index = index;
    return chain_images.at(index);
  }

  [[nodiscard]] Resolution resolution() const noexcept {
    return Resolution {
      .width  = static_cast<std::uint16_t>(extent_info.width),
      .height = static_cast<std::uint16_t>(extent_info.height),
    };
  }

  [[nodiscard]] semaphore_type const& present_semaphore(FrameIndex const index) const noexcept {
    return semaphores.at(index % defaults::max_frames_in_flight).present;
  }

  [[nodiscard]] semaphore_type const& render_semaphore() const noexcept { return semaphores.at(image_index).render; }

  [[nodiscard]] ImageIndex current_image_index() const noexcept { return image_index; }

  [[nodiscard]] native_type const& operator*() const noexcept { return handle; }

private:
  ImageIndex image_index {0};
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

class SwapchainsManager {
public:
  using swapchain_type       = Swapchain;
  using underlying_container = std::vector<swapchain_type>;
  using iterator             = underlying_container::iterator;
  using const_iterator       = underlying_container::const_iterator;

  void push_back(swapchain_type&& swapchain) { swapchains.emplace_back(std::move(swapchain)); }

  template<typename... Args>
    requires(std::constructible_from<swapchain_type, Args...>)
  void emplace_back(Args&&... args) {
    swapchains.emplace_back(std::forward<Args>(args)...);
    wait_destination_stage_mask.emplace_back(core::PipelineStageFlagBits::eColorAttachmentOutput);
  }

  template<typename Self>
  auto begin(this Self&& self) noexcept {
    return std::begin(std::forward<Self>(self).swapchains);
  }

  template<typename Self>
  auto end(this Self&& self) noexcept {
    return std::end(std::forward<Self>(self).swapchains);
  }

  // core::SubmitInfo submit_info(CommandBuffer const& command_buffer) {
  //   // for (auto const& swapchain: swapchains) {
  //   //   auto const image_index = swapchain.current_image_index();
  //   //   swapchain_handles.emplace_back(*swapchain);
  //   //   indices.emplace_back(image_index);
  //   //   wait_semaphores.emplace_back(swapchain.render_semaphore(image_index));
  //   // }
  //
  //   return {
  //     .waitSemaphoreCount = 1,
  //     .pWaitSemaphores      = ,
  //     .pWaitDstStageMask    = &wait_destination_stage_mask,
  //     .commandBufferCount   = 1,
  //     .pCommandBuffers      = &**command_buffer,
  //     .signalSemaphoreCount = 1,
  //     .pSignalSemaphores    = &*swapchain.render_semaphore(image_index)
  //   };
  // }
  //
  // core::PresentInfoKHR present_info(FrameIndex const frame_index) {
  //   handles.clear();
  //   indices.clear();
  //   rendering_semaphores.clear();
  //
  //   for (auto const& swapchain: swapchains) {
  //     auto const image_index = swapchain.current_image_index();
  //     handles.emplace_back(*swapchain);
  //     indices.emplace_back(image_index);
  //     rendering_semaphores.emplace_back(swapchain.render_semaphore());
  //   }
  //
  //   return core::PresentInfoKHR {
  //     .waitSemaphoreCount = static_cast<std::uint32_t>(rendering_semaphores.size()),
  //     .pWaitSemaphores    = rendering_semaphores.data(),
  //     .swapchainCount     = static_cast<std::uint32_t>(handles.size()),
  //     .pSwapchains        = handles.data(),
  //     .pImageIndices      = indices.data(),
  //   };
  // }

  // SwapchainsManager() { swapchains.reserve(4); }

  auto acquire_next_images(FrameIndex const frame_index) {
    return swapchains | std::views::transform([frame_index](auto& swapchain) -> Image const& {
             return swapchain.next_image(frame_index);
           });
  }


  std::vector<core::Semaphore> present_semaphores(FrameIndex const frame_index) {
    auto const get_semaphore = [frame_index](auto const& swapchain) {
      return *swapchain.present_semaphore(frame_index);
    };
    return swapchains //
           | std::views::transform(get_semaphore) //
           | std::ranges::to<std::vector>();
  }

  std::vector<core::Semaphore> render_semaphores() {
    return swapchains //
           | std::views::transform([](auto const& sc) { return *sc.render_semaphore(); }) //
           | std::ranges::to<std::vector>();
  }

  std::vector<core::SwapchainKHR> swapchain_handles() {
    return swapchains //
           | std::views::transform([](auto const& swapchain) { return **swapchain; }) //
           | std::ranges::to<std::vector>();
  }

  std::vector<ImageIndex> image_indices() {
    return swapchains //
           | std::views::transform([](auto const& sc) { return sc.current_image_index(); }) //
           | std::ranges::to<std::vector>();
  }

  std::span<core::PipelineStageFlags> wait_destination_stage_masks() { return wait_destination_stage_mask; }

private:
  mono::stable_vector<swapchain_type> swapchains;
  std::vector<core::PipelineStageFlags> wait_destination_stage_mask;
};


} // namespace rf3d::gfx::vk
