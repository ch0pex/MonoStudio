#include <GLFW/glfw3.h>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <rflect3d/window/utils/resolution.hpp>
#include <rflect3d/window/window_builder.hpp>
#include <rflect3d/window/window_handle.hpp>
#include <rflect3d/window/window_modes.hpp>
#include <rflect3d/window/window_types.hpp>

TEST_SUITE_BEGIN("Window");

using namespace rflect;

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
  WindowHandle window = rflect::WindowBuilder(window_config(WindowMode::exclusive_full_screen)).build();
  CHECK(window.native_handle() != nullptr);
  CHECK(window.resolution() != resolution); // Specified resolution is ignored by this mode
}

TEST_CASE("Full screen borderless") {
  WindowHandle window = rflect::WindowBuilder(window_config(WindowMode::borderless_full_screen)).build();

  CHECK(window.native_handle() != nullptr);
  CHECK(window.resolution() != resolution); // Specified resolution is ignored by this mode
}

TEST_CASE("Windowed borderless") {
  WindowHandle window = rflect::WindowBuilder(window_config(WindowMode::windowed_borderless)).build();

  CHECK(window.native_handle() != nullptr);
  CHECK(window.resolution() == resolution);
}

TEST_CASE("Windowed normal") {
  WindowHandle window = rflect::WindowBuilder(window_config(WindowMode::windowed)).build();

  CHECK(window.resolution() == resolution);
  CHECK(window.native_handle() != nullptr);
}

TEST_SUITE_END();
