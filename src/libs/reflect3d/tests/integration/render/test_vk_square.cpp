#include "common.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>

using namespace rf3d;
using namespace rf3d::gfx;
using namespace rf3d::gfx::vk;

std::array constexpr vertices = {
  Vertex {.position = {-0.5F, -0.5F, 0.F}, .color {1.0F, 0.0F, 0.0F, 0.0F}},
  Vertex {.position = {0.5F, -0.5F, 0.F}, .color = {0.0F, 1.0F, 0.0F, 0.F}},
  Vertex {.position = {0.5F, 0.5F, 0.F}, .color = {0.0F, 0.0F, 1.0F, 0.F}},
  Vertex {.position = {-0.5F, 0.5F, 0.F}, .color = {1.0F, 1.0F, 1.0F, 0.F}}
};

int main() try {
  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");

  Surface surface = test::create_test_surface("Vk Triangle");
  Core renderer {};

  renderer.add_mesh(
      Mesh {
        .vertices = std::ranges::to<std::vector>(vertices),
        .indices  = std::vector<Index> {0, 1, 2, 2, 3, 0},
      }
  );

  while (mono::ex::should_run()) {
    input::poll_events();
    renderer.render_surface(surface, {});
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
