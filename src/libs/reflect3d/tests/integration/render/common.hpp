#pragma once

#include "reflect3d/graphics/vk/vk_core.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"

#include <reflect3d/graphics/vk/vk_surface.hpp>

namespace test {


inline rf3d::gfx::vk::Surface create_test_surface(std::string_view const title) {
  auto window_config = rf3d::config::Window {
    .title = std::string {title},
    .mode  = rf3d::WindowMode::windowed,
  };

  auto window = rf3d::WindowBuilder(window_config).default_callbacks().build(); //
  return rf3d::gfx::vk::Surface {std::move(window)};
}

} // namespace test
