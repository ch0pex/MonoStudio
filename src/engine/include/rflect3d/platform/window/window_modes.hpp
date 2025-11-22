#pragma once

//
#include "rflect3d/platform/window/monitor.hpp"
#include "rflect3d/platform/window/utils/resolution.hpp"
#include "rflect3d/platform/window/window_config.hpp"
#include "rflect3d/platform/window/window_types.hpp"

//
#include <GLFW/glfw3.h>

//
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>

namespace rflect::detail {

NativeWindow create_handle( //
  Resolution const resolution,  //
  std::string const& title,  //
  Monitor monitor = nullptr,  //
  GLFWwindow* share = nullptr //
) {

  NativeWindow window = glfwCreateWindow( //
      resolution.width, //
      resolution.height,  //
      title.c_str(),  //
      monitor,  //
      share //
  );
  if (window == nullptr)
    throw "Couldn't create native window, glfwCreateWindow returned nullptr";

  return window;
}


// --- Fullscreen real ---
NativeWindow create_fullscreen(config::Window const& config, Monitor monitor) {
  GLFWvidmode const* videoMode = glfwGetVideoMode(monitor);
  if (videoMode == nullptr) {
    throw std::runtime_error("Failed to get video mode for monitor");
  }

  for (auto const& [hint, value]: config.hints) {
    glfwWindowHint(hint, value);
  }

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Fullscreen real usa decoración de sistema

  return glfwCreateWindow(videoMode->width, videoMode->height, config.title.c_str(), monitor, nullptr);
}

// --- Fullscreen borderless ---
NativeWindow create_fullscreen_borderless(config::Window const& config, Monitor monitor) {
  GLFWvidmode const* videoMode = glfwGetVideoMode(monitor);

  if (videoMode == nullptr) {
    throw std::runtime_error("Failed to get video mode for monitor");
  }

  for (auto const& [hint, value]: config.hints) {
    glfwWindowHint(hint, value);
  }


  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  NativeWindow window = detail::create_handle(
      {
        .width  = static_cast<uint16_t>(videoMode->width),
        .height = static_cast<uint16_t>(videoMode->height),
      },
      config.title
  );

  if (window == nullptr)
    return nullptr;

  int mx = 0;
  int my = 0;
  glfwGetMonitorPos(monitor, &mx, &my);
  glfwSetWindowPos(window, mx, my);

  return window;
}

// --- Windowed borderless ---
NativeWindow create_windowed_borderless(config::Window const& config, Monitor monitor) {
  for (auto const& [hint, value]: config.hints) {
    glfwWindowHint(hint, value);
  }

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);


  NativeWindow window = detail::create_handle(config.resolution, config.title);

  int mx = 0;
  int my = 0;
  glfwGetMonitorPos(monitor, &mx, &my);
  glfwSetWindowPos(window, mx, my);

  return window;
}

// --- Windowed normal ---
NativeWindow create_windowed(config::Window const& config) {
  for (auto const& [hint, value]: config.hints) {
    glfwWindowHint(hint, value);
  }

  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  return detail::create_handle(config.resolution, config.title);
}

/**
 * @brief Returns a window native handle given a window specification
 */
NativeWindow native_window(config::Window const& config) {
  glfwDefaultWindowHints();

  // Get configured monitor if possible otherwise gets the main monitor
  auto* monitor = get_monitor(config.monitor).value_or(get_monitors()[0]);

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
