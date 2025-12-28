#pragma once

#include "mono/logging/logger.hpp"
#include "reflect3d/window/monitor.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/utils/window_error.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_types.hpp"

#include <GLFW/glfw3.h>

#include <optional>
#include <stdexcept>
#include <utility>


namespace rf3d::detail {

inline void center_window_on_monitor(GLFWwindow* window, GLFWmonitor* monitor, int width, int height) {
  if (monitor == nullptr)
    return;

  int mx = 0;
  int my = 0;
  int mw = 0;
  int mh = 0;
  glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);

  int x = mx + ((mw - width) / 2);
  int y = my + ((mh - height) / 2);

  glfwSetWindowPos(window, x, y);
}

inline void apply_hints(std::ranges::range auto const& hints) {
  glfwDefaultWindowHints();

  for (auto const& [hint, value]: hints)
    glfwWindowHint(hint, value);
}

inline NativeWindow create_handle(
    Resolution const resolution, //
    std::string const& title, //
    NativeMonitor monitor = nullptr, //
    GLFWwindow* share     = nullptr //
) {
  NativeWindow const window = glfwCreateWindow(resolution.width, resolution.height, title.c_str(), monitor, share);

  if (window == nullptr) {
    throw WindowException("Failed to create window: " + get_window_error());
  }

  return window;
}

/**************************
 *       Window Modes     *
 **************************/
inline NativeWindow create_windowed(config::Window const& config, Monitor monitor) {
  apply_hints(config.hints);

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  NativeWindow const window = create_handle(config.resolution, config.title);

  center_window_on_monitor(window, monitor.native_handle(), config.resolution.width, config.resolution.height);

  return window;
}

inline NativeWindow create_windowed_borderless(config::Window const& config, Monitor monitor) {
  apply_hints(config.hints);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  NativeWindow const window = create_handle(config.resolution, config.title, nullptr);
  center_window_on_monitor(window, monitor.native_handle(), config.resolution.width, config.resolution.height);

  return window;
}

NativeWindow create_borderless_fullscreen(config::Window const& config, Monitor const monitor) {
  apply_hints(config.hints);

  // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, monitor.redBits());
  glfwWindowHint(GLFW_GREEN_BITS, monitor.greenBits());
  glfwWindowHint(GLFW_BLUE_BITS, monitor.blueBits());
  glfwWindowHint(GLFW_REFRESH_RATE, monitor.refreshRate());

  Resolution const log_res = monitor.physical_resolution();
  NativeWindow window =
      create_handle({.width = log_res.width, .height = log_res.height}, config.title, monitor.native_handle());

  return window;
}

NativeWindow create_exclusive_fullscreen(config::Window const& config, Monitor const monitor) {
  apply_hints(config.hints);

  NativeWindow window = create_handle(config.resolution, config.title, monitor.native_handle());
  return window;
}

/************************************
 *       Native window builder      *
 ************************************/
inline NativeWindow native_window(config::Window const& config) {
  glfwDefaultWindowHints();

  // clang-format off
  Monitor const monitor = Monitor::from_id(config.monitor).or_else([]() -> std::optional<Monitor> { 
    LOG_WARNING("Selected monitor doesn't exist, primary monitor will be used instead")
    return Monitor::primary();
  }).value(); // clang-format on

  LOG_INFO("Selected monitor name: {}", monitor.name());

  switch (config.mode) {
    case WindowMode::exclusive_full_screen:
      return create_exclusive_fullscreen(config, monitor);
    case WindowMode::borderless_full_screen:
      return create_borderless_fullscreen(config, monitor);
    case WindowMode::windowed_borderless:
      return create_windowed_borderless(config, monitor);
    case WindowMode::windowed:
      return create_windowed(config, monitor);
  }

  throw std::invalid_argument("Specified invalid window mode");
}

} // namespace rf3d::detail
