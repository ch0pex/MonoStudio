#include <GLFW/glfw3.h>
#include "reflect3d/window/monitor.hpp"


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <reflect3d/window/utils/resolution.hpp>
#include <reflect3d/window/window_builder.hpp>
#include <reflect3d/window/window_handle.hpp>
#include <reflect3d/window/window_modes.hpp>
#include <reflect3d/window/window_types.hpp>

TEST_SUITE_BEGIN("Window");

using namespace rf3d;

constexpr Resolution resolution {
  .width  = 800,
  .height = 600,
};

auto constexpr window_config = [](WindowMode const mode) {
  return config::Window {
    .title      = "TestWindow",
    .resolution = resolution,
    .mode       = mode,
  };
};

TEST_CASE("Full screen") {
  WindowHandle window = rf3d::WindowBuilder(window_config(WindowMode::exclusive_full_screen)).build();
  CHECK(window.native_handle() != nullptr);
}

TEST_CASE("Full screen borderless") {
  WindowHandle window = rf3d::WindowBuilder(window_config(WindowMode::borderless_full_screen)).build();
  CHECK(window.native_handle() != nullptr);
}

TEST_CASE("Windowed borderless") {
  WindowHandle window = rf3d::WindowBuilder(window_config(WindowMode::windowed_borderless)).build();

  CHECK(window.native_handle() != nullptr);
  CHECK(window.size() == resolution);
}

TEST_CASE("Windowed normal") {
  WindowHandle window = rf3d::WindowBuilder(window_config(WindowMode::windowed)).build();

  CHECK(window.size() == resolution);
  CHECK(window.native_handle() != nullptr);
}

TEST_SUITE_END();
