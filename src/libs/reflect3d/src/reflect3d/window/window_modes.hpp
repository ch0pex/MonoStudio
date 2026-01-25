#pragma once

#include "reflect3d/window/monitor.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/utils/window_error.hpp"
#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono/logging/logger.hpp>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <optional>
#include <stdexcept>


namespace rf3d::detail {

inline void center_window_on_monitor(NativeWindow const window, Monitor const monitor, int width, int height) {
  MonitorWorkingArea const work_area = monitor.working_area();


  std::uint16_t const x = work_area.position.x + ((work_area.size.width - width) / 2);
  std::uint16_t const y = work_area.position.y + ((work_area.size.height - height) / 2);

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
    std::optional<Monitor> const monitor = std::nullopt
) {
  NativeWindow const window = glfwCreateWindow(resolution.width, resolution.height, title.c_str(), nullptr, nullptr);

  if (window == nullptr) {
    throw WindowException("Failed to create window: " + get_window_error());
  }

  if (monitor.has_value()) {
    monitor->attach_window(window);
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

  center_window_on_monitor(window, monitor, config.resolution.width, config.resolution.height);

  return window;
}

inline NativeWindow create_windowed_borderless(config::Window const& config, Monitor monitor) {
  apply_hints(config.hints);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  NativeWindow const window = create_handle(config.resolution, config.title);
  center_window_on_monitor(window, monitor, config.resolution.width, config.resolution.height);

  return window;
}

inline NativeWindow create_exclusive_fullscreen(config::Window const& config, Monitor const monitor) {
  apply_hints(config.hints);

  NativeWindow window = create_handle(config.resolution, config.title, monitor);
  return window;
}

inline NativeWindow create_borderless_fullscreen(config::Window const& config, Monitor const monitor) {
  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    LOG_INFO("Wayland backend detected, borderless fullscreen fallbacks to exclusive full screen mode");
    return create_exclusive_fullscreen(config, monitor);
  }

  apply_hints(config.hints);

  // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, monitor.redBits());
  glfwWindowHint(GLFW_GREEN_BITS, monitor.greenBits());
  glfwWindowHint(GLFW_BLUE_BITS, monitor.blueBits());
  glfwWindowHint(GLFW_REFRESH_RATE, monitor.refresh_rate());

  Resolution const log_res = monitor.physical_resolution();
  NativeWindow window      = create_handle(log_res, config.title);

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
