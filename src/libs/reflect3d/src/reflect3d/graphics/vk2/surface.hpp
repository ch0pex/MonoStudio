#pragma once

#include "reflect3d/graphics/core2/defaults.hpp"
#include "reflect3d/graphics/core2/primitive_types.hpp"
#include "reflect3d/graphics/vk2/detail/vk_instance.hpp"
#include "reflect3d/graphics/vk2/semaphore.hpp"
#include "reflect3d/graphics/vk2/texture.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/containers/static_vector.hpp>

namespace rf3d::vk {

namespace detail {

struct Image {
  BackBufferTexture back_buffer;
  Semaphore render_semaphore;
  Semaphore present_semaphore;
};

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
  if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  return {
    .width = std::clamp<std::uint32_t>(
        resolution.width, //
        capabilities.minImageExtent.width, //
        capabilities.maxImageExtent.width
    ),
    .height = std::clamp<std::uint32_t>(
        resolution.height, //
        capabilities.minImageExtent.height, //
        capabilities.maxImageExtent.height
    )
  };
}

inline auto get_images(raii::SwapchainKHR const& swapchain, core::SwapchainCreateInfoKHR const& config) {
  core::ImageViewCreateInfo view_info {
    .viewType         = core::ImageViewType::e2D,
    .format           = config.imageFormat,
    .subresourceRange = {
      .aspectMask     = core::ImageAspectFlagBits::eColor,
      .baseMipLevel   = 0,
      .levelCount     = 1,
      .baseArrayLayer = 0,
      .layerCount     = 1
    }
  };

  Resolution const resolution {
    .width  = static_cast<std::uint16_t>(config.imageExtent.width),
    .height = static_cast<std::uint16_t>(config.imageExtent.height),
  };

  auto build_image = [&](core::Image const& img) { //
    view_info.image = img;
    return Image {
      .back_buffer       = {img, make_image_view(view_info), resolution},
      .render_semaphore  = {},
      .present_semaphore = {},
    };
  };

  return swapchain.getImages() //
         | std::views::transform(build_image) //
         | std::ranges::to<std::vector>();
}

inline core::SwapchainCreateInfoKHR create_swapchain_config(
    raii::SurfaceKHR const& surface, //
    SurfaceInfo const& surface_details, //
    Resolution const& resolution //
) {
  auto const [format, colorSpace] = choose_surface_format(surface_details.formats);
  auto const present_mode         = choose_present_mode(surface_details.present_modes);

  return {
    .surface          = *surface,
    .minImageCount    = choose_swap_min_image_count(surface_details.capabilities),
    .imageFormat      = format,
    .imageColorSpace  = colorSpace,
    .imageExtent      = choose_swap_extent(resolution, surface_details.capabilities),
    .imageArrayLayers = 1,
    .imageUsage       = core::ImageUsageFlagBits::eColorAttachment,
    .imageSharingMode = core::SharingMode::eExclusive,
    .preTransform     = surface_details.capabilities.currentTransform,
    .compositeAlpha   = core::CompositeAlphaFlagBitsKHR::eOpaque,
    .presentMode      = present_mode,
    .clipped          = core::True,
  };
}

class Swapchain {
public:
  using image_type  = Image*;
  using handle_type = raii::SwapchainKHR;
  using config_type = core::SwapchainCreateInfoKHR;
  using extent_type = core::Extent2D;

  explicit Swapchain(config_type const& config) :
    handle(make_swapchain(config)), //
    images(get_images(handle, config)), //
    extent(config.imageExtent) //
  { }

  Swapchain(Swapchain const&) = delete;

  Swapchain& operator=(Swapchain const&) = delete;

  Swapchain(Swapchain&& other) noexcept :
    handle((wait_idle(), std::move(other.handle))), //
    images(std::move(other.images)), //
    current_image_index(other.current_image_index), //
    extent(other.extent) //
  { }

  Swapchain& operator=(Swapchain&& other) noexcept {
    if (other.handle != nullptr) {
      handle = std::move(other.handle);
      extent = other.extent;
      images = std::move(other.images);
      wait_idle();
      current_image_index = other.current_image_index;
    }
    return *this;
  }

  ~Swapchain() { //
    if (handle != nullptr) {
      wait_idle();
    }
  }

  // TODO: Not use exception for flow control
  [[nodiscard]] image_type next_image(FrameIndex const frame_index) try {
    auto [result, index] = handle.acquireNextImage( //
      rf3d::defaults::wait_timeout.count(),  //
      *images.at(frame_index).present_semaphore.handle(),  //
      nullptr //
    );
    current_image_index = index;
    return std::addressof(images.at(index));
  }
  catch (std::exception const& e) {
    LOG_INFO("Failed to acquire image: {}", e.what());
    return nullptr;
  }

  [[nodiscard]] Semaphore const& present_semaphore(FrameIndex const frame_index) const noexcept {
    return images.at(frame_index).present_semaphore;
  }

  [[nodiscard]] Semaphore const& render_semaphore() const noexcept {
    return images.at(current_image_index).render_semaphore;
  }

  [[nodiscard]] Resolution resolution() const noexcept {
    return Resolution {
      .width  = static_cast<std::uint16_t>(extent.width),
      .height = static_cast<std::uint16_t>(extent.height),
    };
  }

  void recreate(config_type config) {
    config.oldSwapchain    = *handle;
    handle_type new_handle = make_swapchain(config);
    handle                 = std::move(new_handle);
    extent                 = config.imageExtent;
    images                 = get_images(handle, config);
    current_image_index    = 0;
  }

  [[nodiscard]] core::PresentInfoKHR present_info() const {
    return core::PresentInfoKHR {
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = std::addressof(*render_semaphore().handle()),
      .swapchainCount     = 1,
      .pSwapchains        = std::addressof(*handle),
      .pImageIndices      = std::addressof(current_image_index),
    };
  }

private:
  handle_type handle;
  std::vector<Image> images;
  ImageIndex current_image_index {};
  extent_type extent;
};

} // namespace detail


/**
 * @brief The Surface class encapsulates the Vulkan surface and swapchain management for a given window.
 *
 * This class is responsible for creating the Vulkan surface from the provided window, managing the swapchain,
 * the swapchain images can be accessed through the `next_image` method,
 * which returns a pointer to the back buffer texture for the current frame index.
 *
 */
class Surface {
public:
  // --- Type traits ---

  using resolution_type = rf3d::Resolution;
  using image_type      = BackBufferTexture*;
  using window_type     = Window;
  using handle_type     = detail::raii::SurfaceKHR;
  using swapchain_type  = detail::Swapchain;

  // --- Constructors ---

  explicit Surface(window_type&& window_surface) :
    window(std::move(window_surface)), //
    surface_handle(detail::create_surface(window.native_handle())), //
    swapchain(
        detail::create_swapchain_config(
            surface_handle, //
            detail::get_surface_info(surface_handle), //
            window.resolution() //
        )
    ) { }

  Surface(Surface const&) = delete;

  Surface(Surface&&) = default;

  Surface& operator=(Surface const&) = delete;

  Surface& operator=(Surface&&) = default;

  ~Surface() {
    if (surface_handle != nullptr) {
      detail::wait_idle();
    }
  }

  // --- Member functions ---

  [[nodiscard]] image_type next_image(FrameIndex const frame_index) {
    assert(frame_index < defaults::max_frames_in_flight);
    if (auto* image = swapchain.next_image(frame_index)) {
      return std::addressof(image->back_buffer);
    }
    recreate_swapchain();
    return nullptr;
  }

  [[nodiscard]] Resolution window_size() const { //
    return window.resolution();
  }

  [[nodiscard]] Resolution resolution() const { //
    return swapchain.resolution();
  }

  [[nodiscard]] Viewport viewport() const { //
    auto res = resolution();
    return Viewport {
      .rect = {
        .extent = {res.width, res.height},
      },
    };
  }

  [[nodiscard]] Semaphore const& render_semaphore() const { //
    return swapchain.render_semaphore();
  }

  [[nodiscard]] Semaphore const& present_semaphore(FrameIndex const frame_index) const {
    return swapchain.present_semaphore(frame_index);
  }

  void present() { //
    std::ignore = detail::present(swapchain.present_info()).or_else([&](auto const& e) -> mono::expected<void> {
      recreate_swapchain();
      LOG_WARNING("Failed to present swapchain image: {}", e.what());
      return {};
    });
  }

private:
  void recreate_swapchain() {
    LOG_INFO("Recreating swapchain");
    while (window_size() == null_resolution) {
      glfwWaitEvents();
    }
    detail::wait_idle();
    swapchain.recreate(
        detail::create_swapchain_config(
            surface_handle, //
            detail::get_surface_info(surface_handle), //
            window.resolution() //
        )
    );
  }

  window_type window;
  handle_type surface_handle;
  swapchain_type swapchain;
};

} // namespace rf3d::vk
