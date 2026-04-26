#pragma once

#include "reflect3d/graphics/core/defaults.hpp"
#include "reflect3d/graphics/core/primitive_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_instance.hpp"
#include "reflect3d/graphics/vk/semaphore.hpp"
#include "reflect3d/graphics/vk/texture.hpp"
#include "reflect3d/window/utils/callbacks.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

#include <chrono>
#include <mono/containers/static_vector.hpp>

#include <array>

namespace rf3d::vk {

namespace detail {

struct Image {
  BackBufferTexture back_buffer;
};

struct FrameSync { };

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

  // return core::PresentModeKHR::eImmediate; // FIFO is guaranteed to be supported

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
      .back_buffer = {img, make_image_view(view_info), resolution},
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
    swapchain_handle(make_swapchain(config)), //
    images(get_images(swapchain_handle, config)), //
    render_semaphores(images.size()), //
    extent(config.imageExtent) //
  { }

  Swapchain(Swapchain const&) = delete;

  Swapchain& operator=(Swapchain const&) = delete;

  Swapchain(Swapchain&& other) noexcept :
    swapchain_handle((wait_idle(), std::move(other.swapchain_handle))), //
    images(std::move(other.images)), //
    present_semaphores(std::move(other.present_semaphores)), //
    render_semaphores(std::move(other.render_semaphores)), //
    current_image_index(other.current_image_index), //
    extent(other.extent) //
  { }

  Swapchain& operator=(Swapchain&& other) noexcept {
    if (other.swapchain_handle != nullptr) {
      swapchain_handle = std::move(other.swapchain_handle);
      extent           = other.extent;
      images           = std::move(other.images);
      wait_idle();
      present_semaphores  = std::move(other.present_semaphores);
      render_semaphores   = std::move(other.render_semaphores);
      current_image_index = other.current_image_index;
    }
    return *this;
  }

  ~Swapchain() { //
    if (swapchain_handle != nullptr) {
      wait_idle();
    }
  }

  // TODO: Not use exception for flow control
  [[nodiscard]] image_type next_image() try {
    auto [result, index] = swapchain_handle.acquireNextImage( //
      rf3d::defaults::wait_timeout.count(),  //
      *present_semaphores.at(frame_index().value()).handle(),  //
      nullptr //
    );
    if (result != core::Result::eSuccess && result != core::Result::eSuboptimalKHR) {
      LOG_INFO("Failed to acquire image: {}", to_string(result));
      return nullptr;
    }
    current_image_index = index;
    return std::addressof(images.at(index));
  }
  catch (std::exception const& e) {
    LOG_INFO("Failed to acquire image: {}", e.what());
    return nullptr;
  }

  [[nodiscard]] Semaphore const& present_semaphore() const noexcept {
    return present_semaphores.at(frame_index().value());
  }

  [[nodiscard]] Semaphore const& render_semaphore() const noexcept { return render_semaphores.at(current_image_index); }

  [[nodiscard]] Resolution resolution() const noexcept {
    return Resolution {
      .width  = static_cast<std::uint16_t>(extent.width),
      .height = static_cast<std::uint16_t>(extent.height),
    };
  }

  void recreate(config_type config) {
    config.oldSwapchain    = *swapchain_handle;
    handle_type new_handle = make_swapchain(config);
    swapchain_handle       = std::move(new_handle);
    extent                 = config.imageExtent;
    images                 = get_images(swapchain_handle, config);
    current_image_index    = 0;
    render_semaphores      = std::vector<Semaphore>(images.size());
  }

  [[nodiscard]] core::PresentInfoKHR present_info() const {
    return core::PresentInfoKHR {
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = std::addressof(*render_semaphores.at(current_image_index).handle()),
      .swapchainCount     = 1,
      .pSwapchains        = std::addressof(*swapchain_handle),
      .pImageIndices      = std::addressof(current_image_index),
    };
  }

  [[nodiscard]] ImageIndex image_index() const noexcept { return current_image_index; }

  [[nodiscard]] handle_type const& handle() const noexcept { return swapchain_handle; }

private:
  handle_type swapchain_handle;
  std::vector<Image> images;
  std::array<Semaphore, rf3d::defaults::max_frames_in_flight> present_semaphores {};
  std::vector<Semaphore> render_semaphores {};
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

  using resolution_type = Resolution;
  using image_type      = BackBufferTexture*;
  using window_type     = Window;
  using handle_type     = detail::raii::SurfaceKHR;
  using swapchain_type  = detail::Swapchain;

  // --- Constructors ---

  explicit Surface(window_type&& window_surface) : //
    window(std::move(window_surface)), //
    surface_handle(detail::create_surface(window.handle())), //
    sc(detail::create_swapchain_config(
        surface_handle, //
        detail::get_surface_info(surface_handle), //
        window.resolution() //
    )) //
  { }

  Surface(Surface const&) = delete;

  Surface(Surface&&) = default;

  Surface& operator=(Surface const&) = delete;

  Surface& operator=(Surface&&) = default;

  ~Surface() {
    // TODO: think about this wait_idle
    // if (surface_handle != nullptr) {
    //   detail::wait_idle();
    // }
  }

  // --- Member functions ---

  [[nodiscard]] image_type next_image() {
    if (window_size() == null_resolution) {
      return nullptr;
    }
    if (auto* image = sc.next_image(); image != nullptr) {
      return std::addressof(image->back_buffer);
    }
    recreate_swapchain();
    return nullptr;
  }

  void resize(resolution_type const& new_resolution) {
    if (new_resolution != window_size()) {
      window.resize(new_resolution);
      recreate_swapchain();
    }
  }

  [[nodiscard]] Resolution window_size() const { //
    return window.resolution();
  }

  [[nodiscard]] Resolution resolution() const { //
    return sc.resolution();
  }

  [[nodiscard]] Viewport viewport() const { //
    auto const [width, height] = resolution();
    return Viewport {
      .rect = {.extent = {width, height}},
    };
  }

  [[nodiscard]] Semaphore const& render_semaphore() const { //
    return sc.render_semaphore();
  }

  [[nodiscard]] Semaphore const& present_semaphore() const { return sc.present_semaphore(); }

  [[nodiscard]] ImageIndex image_index() const { //
    return sc.image_index();
  }

  void present() { //
    std::ignore = detail::present(sc.present_info()).or_else([&](auto const& /*error*/) -> mono::expected<void> {
      recreate_swapchain();
      return {};
    });
  }

  [[nodiscard]] swapchain_type const& swapchain() const noexcept { return sc; }

  void recreate_swapchain() {
    if (window_size() == null_resolution) {
      return;
    }
    detail::wait_idle();
    LOG_INFO("Recreating swapchain");
    sc.recreate(
        detail::create_swapchain_config(
            surface_handle, //
            detail::get_surface_info(surface_handle), //
            window.resolution() //
        )
    );
  }

private:
  window_type window;
  handle_type surface_handle;
  swapchain_type sc;
};

} // namespace rf3d::vk
