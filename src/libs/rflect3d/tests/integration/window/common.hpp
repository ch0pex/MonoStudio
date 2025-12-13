#pragma once


#include "rflect3d/core/execution/signals.hpp"
#include "rflect3d/core/execution/stop_token.hpp"
#include "rflect3d/input/input.hpp"
#include "rflect3d/window/window.hpp"
#include "rflect3d/window/window_builder.hpp"
#include "rflect3d/window/window_config.hpp"
#include "rflect3d/window/window_types.hpp"

inline void test_window(rflect::WindowMode const mode, std::uint8_t const monitor_id) {
  rflect::config::Window window_spec {.mode = mode, .monitor = monitor_id};

  rflect::Window window {rflect::WindowBuilder(window_spec).build()};

  rflect::ex::setup_signals();

  while (rflect::ex::should_run()) {
    rflect::input::poll_events();
    glfwSwapBuffers(window.native_handle());
  }
}
