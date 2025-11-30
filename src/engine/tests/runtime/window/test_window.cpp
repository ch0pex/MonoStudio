#include "rflect3d/window/utils/defaults.hpp"
#include <rflect3d/window/window.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Window");

TEST_CASE("One window") {
  rflect::Window window = rflect::defaults::make_window();
  CHECK_FALSE(window.should_close());
  CHECK(window.resolution() == rflect::Resolution{});
}

TEST_CASE("Two window") {
  rflect::Window window = rflect::defaults::make_window();
  rflect::Window window2 = rflect::defaults::make_window(
      {.title = "Window2", .resolution{800, 800}});

  CHECK(window.resolution() == rflect::Resolution{});

  CHECK(window.title() == "Rflect");
  CHECK(window2.title() == "Window2");
  CHECK(window2.resolution() == rflect::Resolution{800, 800});
}

TEST_CASE("Move constructor and assignment") {
  rflect::Window window = rflect::defaults::make_window(
      {.title = "Window1", .resolution{800, 800}});
  auto handle = window.native_handle();

  rflect::Window window2{std::move(window)};

  CHECK(window2.native_handle() == handle);
  CHECK(window2.title() == "Window1");

  rflect::Window window3 = rflect::defaults::make_window();

  window3 = std::move(window2);
  CHECK(window3.native_handle() == handle);
  CHECK(window3.title() == "Window1");
}

TEST_SUITE_END();
