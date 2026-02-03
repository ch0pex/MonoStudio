#pragma once

#include "reflect3d/graphics/vk/vk_swapchain.hpp"

namespace rf3d::gfx::vk {
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

    auto const image = swapchain.next_image(frame_index);
    if (not image) {
      recreate_swapchain();
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

  [[nodiscard]] Resolution window_size() const { return window_handle.resolution(); }

  [[nodiscard]] Resolution resolution() const { return swapchain.resolution(); }

  void present() try { gpu::present(swapchain.present_info()); }
  catch (std::exception const& e) {
    recreate_swapchain();
  }

private:
  void recreate_swapchain() {
    LOG_INFO("Recreating swapchain");
    while (window_size() == Resolution {0, 0}) {
      glfwWaitEvents();
    }
    gpu::wait_idle();
    swapchain.recreate(create_swapchain_config(
        window_surface, //
        gpu::get_surface_info(window_surface), //
        window_handle.resolution() //
    ));
  }

  window_type window_handle;
  surface_type window_surface;
  Swapchain swapchain;
};

} // namespace rf3d::gfx::vk
