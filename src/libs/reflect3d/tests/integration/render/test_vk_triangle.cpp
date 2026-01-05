

#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

#include <iostream>

int main() try {
  using namespace rf3d;
  mono::ex::setup_signals();

  gfx::vk::Instance instance {};
  gfx::vk::Gpu gpu = instance.create_gpu();

  auto window = rf3d::WindowBuilder() //
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
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
  return EXIT_FAILURE;
}
