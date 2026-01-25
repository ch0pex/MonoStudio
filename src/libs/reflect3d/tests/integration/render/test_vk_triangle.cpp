#include "reflect3d/graphics/vk/vk_core.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>

int main() try {
  using namespace rf3d;
  using namespace rf3d::gfx::vk;
  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");

  std::vector<Surface> surfaces;
  Core renderer {};

  for (std::size_t i = 0; i < 2; ++i) {
    auto window_config = config::Window {
      .title = std::format("Window {}", i),
      .mode  = WindowMode::windowed,
    };

    auto window = WindowBuilder(window_config).default_callbacks().build(); //
    surfaces.emplace_back(std::move(window));
  }

  while (mono::ex::should_run()) {
    input::poll_events();

    for (auto& surface: surfaces) {
      renderer.render_surface(surface, {});
      // render_surface(surface);
    }
  }
  gpu::wait_idle();
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
  return EXIT_FAILURE;
}
