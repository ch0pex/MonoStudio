
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

int main() {
  using namespace rf3d;
  gfx::vk::Instance instance {};
  gfx::vk::Gpu gpu = instance.create_gpu();

  // gfx::Renderer<hri::vk::Traits> renderer {hri::vk::Instance {}};

  auto window = rf3d::WindowBuilder() //
                                      // .attach_surface(renderer) //
                    .default_callbacks() // //
                    .build(); //


  auto surface = window.create_surface(instance);
  mono::ex::setup_signals();

  while (mono::ex::should_run()) {
    rf3d::input::poll_events();
    window.swap_buffers();
  }
}
