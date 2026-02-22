#pragma once

#include "reflect3d/graphics/rhi.hpp"
#include "reflect3d/graphics/rhi_concepts.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <reflect3d/graphics/vk/vk_surface.hpp>

namespace test {

template<rf3d::gfx::RenderHardwareInterface rhi>
rhi::surface_type create_test_surface(std::string_view title) {
  auto window_config = rf3d::config::Window {
    .title = std::string {title},
    .mode  = rf3d::WindowMode::windowed,
  };

  auto window = rf3d::WindowBuilder(window_config).default_callbacks().build(); //
  return typename rhi::surface_type {std::move(window)};
}

} // namespace test
