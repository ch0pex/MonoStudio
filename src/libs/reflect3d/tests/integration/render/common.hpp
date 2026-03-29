#pragma once

#include <reflect3d/graphics/api/buffer.hpp>
#include <reflect3d/graphics/concepts.hpp>
#include <reflect3d/graphics/core/mesh.hpp>
#include <reflect3d/graphics/vk/vk_surface.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window_builder.hpp>

//
#include <mono/execution/signals.hpp>

//
#include <cstdint>
#include <iostream>

namespace test {

rf3d::Window create_test_window(std::string_view title) {
  auto window_config = rf3d::config::Window {
    .title = std::string {title},
    .mode  = rf3d::WindowMode::windowed,
  };

  return rf3d::WindowBuilder(window_config).default_callbacks().build();
}

template<rf3d::gfx::RenderHardwareInterface rhi>
void renderer(std::uint8_t const num_surfaces, mono::span<rf3d::gfx::Mesh const> meshes) try {
  using namespace rf3d;

  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");

  std::vector<typename rhi::Surface> surfaces {};

  // create surfaces
  surfaces.reserve(num_surfaces);
  for (std::uint8_t i = 0; i < num_surfaces; ++i) {
    surfaces.emplace_back(test::create_test_window("Surface " + std::to_string(i + 1)));
  }

  // add meshes to the renderer
  for (auto const& mesh: meshes) {
    rhi::add_mesh(mesh);
  }

  while (not surfaces.empty()) {
    input::poll_events();
    std::erase_if(surfaces, [](typename rhi::Surface& surface) { return surface.should_close(); });

    for (auto& surface: surfaces) {
      surface.render({});
    }
  }
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
}

} // namespace test
