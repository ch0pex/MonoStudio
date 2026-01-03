
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

int main() {
  using namespace rf3d;
  mono::ex::setup_signals();

  gfx::vk::Instance instance {};
  gfx::vk::Gpu gpu = instance.create_gpu();

  // gfx::Renderer<hri::vk::Traits> renderer {hri::vk::Instance {}};

  auto window = rf3d::WindowBuilder() //
                                      // .attach_surface(renderer) //
                    .default_callbacks() // //
                    .build(); //


  gfx::vk::Swapchain swapchain = gpu.create_swap_chain(
      window.create_surface(instance), //
      window.resolution() //
  );

  while (mono::ex::should_run()) {
    rf3d::input::poll_events();
    window.swap_buffers();
  }
}
