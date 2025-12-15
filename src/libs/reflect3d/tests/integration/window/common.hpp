#pragma once


#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

inline void test_window(rf3d::WindowMode const mode, std::uint8_t const monitor_id) {
  rf3d::config::Window window_spec {.mode = mode, .monitor = monitor_id};

  rf3d::Window window {rf3d::WindowBuilder(window_spec).build()};

  mono::ex::setup_signals();

  while (mono::ex::should_run()) {
    rf3d::input::poll_events();
    glfwSwapBuffers(window.native_handle());
  }
}
