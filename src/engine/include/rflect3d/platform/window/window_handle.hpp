#pragma once

#include "rflect3d/core/logging/logger.hpp"
#include "rflect3d/core/misc/passkey.hpp"
#include "rflect3d/platform/window/monitor.hpp"
#include "rflect3d/platform/window/utils/context.hpp"
#include "rflect3d/platform/window/utils/resolution.hpp"
#include "rflect3d/platform/window/window_types.hpp"

#include <GLFW/glfw3.h>

namespace rflect {

class WindowBuilder;

class WindowHandle {
public:
  using native_type = GLFWwindow*;

  WindowHandle(native_type handle, PassKey<WindowBuilder> /*unused*/) : handle(handle) { }

  WindowHandle(WindowHandle&& other) noexcept : handle(other.handle) { other.handle = nullptr; }

  WindowHandle(WindowHandle const& other) = default;

  WindowHandle& operator=(WindowHandle const& other) = default;

  WindowHandle& operator=(WindowHandle&& other) noexcept {
    if (&other == this) {
      return *this;
    }
    handle       = other.handle;
    other.handle = nullptr;
    return *this;
  }

  bool should_close() { return glfwWindowShouldClose(handle) != 0; }

  [[nodiscard]] std::string_view title() const { return {glfwGetWindowTitle(handle)}; }

  [[nodiscard]] Resolution resolution() const {
    int width  = 0;
    int height = 0;
    glfwGetWindowSize(handle, &width, &height);
    return {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)};
  }

  [[nodiscard]] native_type native_handle() const { return handle; }

  void show() { glfwShowWindow(handle); }

  void hide() { glfwHideWindow(handle); }

  void full_screen(MonitorId const monitor_id) {
    auto monitor = get_monitor(monitor_id);
    if (not monitor) {
      LOG_WARNING("Couldn't set window in full screen mode, monitor with id {} doesn't exist", monitor_id);
      return;
    }

    auto const* mode = glfwGetVideoMode(monitor.value());

    glfwSetWindowMonitor(handle, monitor.value(), 0, 0, mode->width, mode->height, mode->refreshRate);
    glfwSetWindowAttrib(handle, GLFW_DECORATED, false);
    glfwSetWindowAttrib(handle, GLFW_RED_BITS, mode->redBits);
    glfwSetWindowAttrib(handle, GLFW_GREEN_BITS, mode->greenBits);
    glfwSetWindowAttrib(handle, GLFW_BLUE_BITS, mode->blueBits);
    glfwSetWindowAttrib(handle, GLFW_REFRESH_RATE, mode->refreshRate);
  }

  void windowed() {
    auto res = resolution();
    glfwSetWindowMonitor(handle, nullptr, 0, 0, res.width, res.height, 0);
    glfwSetWindowAttrib(handle, GLFW_DECORATED, true);
  }

  [[nodiscard]] bool is_full_screen() const { return glfwGetWindowMonitor(handle) != nullptr; }

private:
  native_type handle {};
};

} // namespace rflect
