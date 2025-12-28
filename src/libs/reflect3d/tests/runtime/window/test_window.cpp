#include <reflect3d/window/window.hpp>
#include "reflect3d/window/utils/defaults.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Window");

TEST_CASE("One window") {
  rf3d::Window window = rf3d::defaults::make_window();
  CHECK_FALSE(window.should_close());
  CHECK(window.size() == rf3d::Resolution {});
}

TEST_CASE("Two window") {
  rf3d::Window window  = rf3d::defaults::make_window();
  rf3d::Window window2 = rf3d::defaults::make_window({.title = "Window2", .resolution {800, 800}});

  CHECK(window.size() == rf3d::Resolution {});

  CHECK(window.title() == "Rflect");
  CHECK(window2.title() == "Window2");
  CHECK(window2.size() == rf3d::Resolution {800, 800});
}

TEST_CASE("Move constructor and assignment") {
  rf3d::Window window = rf3d::defaults::make_window({.title = "Window1", .resolution {800, 800}});
  // auto handle           = window.native_handle();

  rf3d::Window window2 {std::move(window)};

  // CHECK(window2.native_handle() == handle);
  CHECK(window2.title() == "Window1");

  rf3d::Window window3 = rf3d::defaults::make_window();

  window3 = std::move(window2);
  // CHECK(window3.native_handle() == handle);
  CHECK(window3.title() == "Window1");
}

TEST_SUITE_END();
