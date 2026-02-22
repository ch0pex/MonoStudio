#include "common.hpp"
#include "reflect3d/graphics/rhi.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>
#include <stack>
#include <vector>

template<rf3d::gfx::RenderHardwareInterface rhi>
void test_surfaces() try {
  using namespace rf3d;
  using namespace rf3d::gfx;

  using Surface = typename rhi::surface_type;

  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");

  auto window_config = rf3d::config::Window {
    .title = std::string {"Test"},
    .mode  = rf3d::WindowMode::windowed,
  };

  std::vector<Surface> surfaces {};

  surfaces.reserve(3);
  for (std::uint8_t i = 0; i < 3; ++i) {
    surfaces.emplace_back(test::create_test_surface<rhi>(std::format("Window {}", i)));
  }

  std::vector<Index> indices {0, 1, 2, 2, 3, 0};

  while (not surfaces.empty()) {
    input::poll_events();
    std::erase_if(surfaces, [](auto& surface) { return surface.should_close(); });

    for (auto& surface: surfaces) {
      rhi::render_surface(surface, {});
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
