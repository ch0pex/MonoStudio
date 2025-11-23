#pragma once

#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include "rflect3d/core/execution/signals.hpp"
#include "rflect3d/input/input.hpp"
#include "rflect3d/platform/window/window.hpp"
#include "rflect3d/platform/window/window_builder.hpp"
#include "rflect3d/platform/window/window_config.hpp"
#include "rflect3d/platform/window/window_types.hpp"

inline void test_window(rflect::WindowMode const mode, std::uint8_t const monitor_id) {
  rflect::config::Window window_spec {.mode = mode, .monitor = monitor_id};

  rflect::Window window {rflect::WindowBuilder(window_spec).build()};

  auto const token = rflect::ex::setup_signals();
  std::size_t count {0};

  while (not token.stop_requested()) {
    rflect::input::poll_events();
    glfwSwapBuffers(window.native_handle());
    if ((count++) == 0U)
      window.windowed();
  }
}
