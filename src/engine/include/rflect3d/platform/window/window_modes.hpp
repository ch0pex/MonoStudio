#pragma once

#include "rflect3d/platform/window/monitor.hpp"
#include "rflect3d/platform/window/utils/resolution.hpp"
#include "rflect3d/platform/window/window_config.hpp"
#include "rflect3d/platform/window/window_types.hpp"

#include <GLFW/glfw3.h>

#include <optional>
#include <span>
#include <stdexcept>
#include <utility>


namespace rflect::detail {


inline NativeWindow create_handle(
    Resolution const resolution, std::string const& title, Monitor monitor = nullptr, GLFWwindow* share = nullptr
) {
  NativeWindow const window = glfwCreateWindow(resolution.width, resolution.height, title.c_str(), monitor, share);

  if (window == nullptr)
    throw std::runtime_error("no se pudo crear la ventana nativa");

  return window;
}

inline NativeWindow create_fullscreen(config::Window const& config, Monitor monitor) {
  glfwDefaultWindowHints();

  GLFWvidmode const* videoMode = glfwGetVideoMode(monitor);
  if (videoMode == nullptr)
    throw std::runtime_error("no se pudo obtener el modo de video del monitor");

  for (auto const& [hint, value]: config.hints)
    glfwWindowHint(hint, value);
  return glfwCreateWindow(videoMode->width, videoMode->height, config.title.c_str(), monitor, nullptr);
}

inline NativeWindow create_fullscreen_borderless(config::Window const& config, Monitor monitor) {
  glfwDefaultWindowHints();

  GLFWvidmode const* videoMode = glfwGetVideoMode(monitor);
  if (videoMode == nullptr)
    throw std::runtime_error("no se pudo obtener modo de video del monitor");

  for (auto const& [hint, value]: config.hints)
    glfwWindowHint(hint, value);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

  NativeWindow const window = glfwCreateWindow(
      videoMode->width, videoMode->height, config.title.c_str(),

      nullptr, nullptr
  );

  if (window == nullptr)
    throw std::runtime_error("no se pudo crear la ventana borderless");

  int mx = 0;
  int my = 0;
  glfwGetMonitorPos(monitor, &mx, &my);
  glfwSetWindowPos(window, mx, my);

  return window;
}

inline NativeWindow create_windowed_borderless(config::Window const& config, Monitor monitor) {
  glfwDefaultWindowHints();

  for (auto const& [hint, value]: config.hints)
    glfwWindowHint(hint, value);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  NativeWindow const window = create_handle(config.resolution, config.title);

  int mx = 0;
  int my = 0;
  glfwGetMonitorPos(monitor, &mx, &my);
  glfwSetWindowPos(window, mx, my);

  return window;
}

inline NativeWindow create_windowed(config::Window const& config) {
  glfwDefaultWindowHints();

  for (auto const& [hint, value]: config.hints)
    glfwWindowHint(hint, value);

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  return create_handle(config.resolution, config.title);
}

inline NativeWindow native_window(config::Window const& config) {
  glfwDefaultWindowHints();

  Monitor const monitor = get_monitor(config.monitor).value_or(get_monitors()[0]);

  switch (config.mode) {
    case WindowMode::full_screen:
      return create_fullscreen(config, monitor);
    case WindowMode::full_screen_borderless:
      return create_fullscreen_borderless(config, monitor);
    case WindowMode::windowed_borderless:
      return create_windowed_borderless(config, monitor);
    case WindowMode::windowed:
      return create_windowed(config);
  }

  std::unreachable();
}

} // namespace rflect::detail
