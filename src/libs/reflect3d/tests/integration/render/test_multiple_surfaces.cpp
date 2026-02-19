#include "common.hpp"
#include "reflect3d/graphics/hri.hpp"
#include "reflect3d/input/input.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>

template<rf3d::gfx::Hri hri>
void test_surfaces() try {
  using namespace rf3d;
  using namespace rf3d::gfx;


  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");


  std::array surfaces {
    test::create_test_surface<hri>("Surface 1"),
    test::create_test_surface<hri>("Surface 2"),
    test::create_test_surface<hri>("Surface 3"),
    test::create_test_surface<hri>("Surface 4"),
  };

  typename hri::renderer gfx {};
  std::vector<Index> indices {0, 1, 2, 2, 3, 0};

  while (mono::ex::should_run()) {
    input::poll_events();

    for (auto& surface: surfaces) {
      gfx.render_surface(surface, {});
    }
  }
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
}

int main() {
  test_surfaces<rf3d::gfx::Vulkan>();
  // test_surfaces<rf3d::gfx::Dx12>();
}
