#include "common.hpp"

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

  std::array<Surface, 4> surfaces {
    test::create_test_surface("Surface 1"),
    test::create_test_surface("Surface 2"),
    test::create_test_surface("Surface 3"),
    test::create_test_surface("Surface 4"),
  };

  Core gfx {};

  while (mono::ex::should_run()) {
    input::poll_events();

    for (auto& surface: surfaces) {
      gfx.render_surface(surface, {});
    }
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
