#include <GLFW/glfw3.h>
#include "rflect3d/window/monitor.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <rflect3d/window/window_modes.hpp>

TEST_SUITE_BEGIN("Window");
//
// TEST_CASE("No context") {
//   CHECK_THROWS([]() { auto _ = rflect::detail::create_windowed({}); }());
// }

TEST_CASE("Full screen") {
  using namespace rflect;
  glfwInit();
  auto monitor = Monitor::from_id(0).value();


  config::Window cfg;

  auto* window = rflect::detail::create_exclusive_fullscreen(cfg, monitor);
  CHECK(window != nullptr);
}

// TEST_CASE("Full screen borderless") {
//   using namespace rflect;
//   glfwInit();
//
//   auto* monitor = get_monitor(0).value();
//   config::Window cfg;
//
//   auto* window = rflect::detail::create_fullscreen_borderless(cfg, monitor);
//   CHECK(window != nullptr);
// }
//
// TEST_CASE("Windowed borderless") {
//   using namespace rflect;
//   glfwInit();
//
//   auto* monitor = get_monitor(0).value();
//   config::Window cfg;
//
//   auto* window = rflect::detail::create_windowed_borderless(cfg, monitor);
//   CHECK(window != nullptr);
// }
//
// TEST_CASE("Windowed normal") {
//   using namespace rflect;
//   glfwInit();
//
//   config::Window cfg;
//
//   auto* window = rflect::detail::create_windowed(cfg);
//   CHECK(window != nullptr);
// }

TEST_SUITE_END();
