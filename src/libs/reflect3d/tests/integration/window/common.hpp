#pragma once


#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono-core/execution/signals.hpp>
#include <mono-core/execution/stop_token.hpp>

inline void test_window(rflect::WindowMode const mode, std::uint8_t const monitor_id) {
  rflect::config::Window window_spec {.mode = mode, .monitor = monitor_id};

  rflect::Window window {rflect::WindowBuilder(window_spec).build()};

  rflect::ex::setup_signals();

  while (rflect::ex::should_run()) {
    rflect::input::poll_events();
    glfwSwapBuffers(window.native_handle());
  }
}
