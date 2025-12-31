

#include <mono/execution/stop_token.hpp>

#include "mono/execution/signals.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/graphics/vk/vk_traits.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/render/renderer.hpp"
#include "reflect3d/window/window_builder.hpp"


int main() {
  using namespace rf3d;
  hri::vk::Instance instance {};
  hri::vk::Gpu gpu = instance.pick_gpu(rf3d::hri::vk::best_gpu_criteria);

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
