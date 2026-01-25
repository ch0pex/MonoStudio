#pragma once

#include "reflect3d/window/monitor.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace rf3d {

class WindowBuilder;

class Window {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using native_type     = GLFWwindow*;
  using position_type   = glm::u16vec2;
  using resolution_type = Resolution;

  /**********************
   *    Constructors    *
   **********************/

  Window(native_type handle, mono::PassKey<WindowBuilder> /*unused*/) : handle(handle) { }

  Window(Window const& other) = delete;

  Window& operator=(Window const& other) = delete;

  Window(Window&& other) noexcept : handle(other.handle) { other.handle = nullptr; }

  Window& operator=(Window&& other) noexcept {
    if (&other == this) {
      return *this;
    }
    handle       = other.handle;
    other.handle = nullptr;
    return *this;
  }

  ~Window() {
    if (handle != nullptr) {
      glfwDestroyWindow(handle);
    }
  }

  /***************************
   *     Member functions    *
   ***************************/

  [[nodiscard]] std::string_view title() const { return {glfwGetWindowTitle(handle)}; }

  void title(std::string const& name) const { glfwSetWindowTitle(handle, name.c_str()); }

  [[nodiscard]] bool should_close() const { return glfwWindowShouldClose(handle) != 0; }

  void swap_buffers() const { glfwSwapBuffers(handle); }

  // --- Position - Size - Resolution ---

  void position(position_type const position) const {
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      LOG_WARNING("Wayland backend detected, setting window position is not supported");
      return;
    }
    glfwSetWindowPos(handle, position.x, position.y);
  }

  [[nodiscard]] position_type position() const {
    int x = 0;
    int y = 0;
    glfwGetWindowPos(handle, &x, &y);
    return {static_cast<std::uint16_t>(x), static_cast<std::uint16_t>(y)};
  }

  [[nodiscard]] resolution_type size() const {
    int width  = 0;
    int height = 0;
    glfwGetWindowSize(handle, &width, &height);
    return {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)};
  }

  void size(resolution_type const size) const { glfwSetWindowSize(handle, size.width, size.height); }

  [[nodiscard]] resolution_type resolution() const {
    int width  = 0;
    int height = 0;
    glfwGetFramebufferSize(handle, &width, &height);
    return {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)};
  }

  // --- Surface ---

  void make_context_current() const { glfwMakeContextCurrent(handle); }


  // void resolution(resolution_type const resolution) const {
  //   int width  = 0;
  //   int height = 0;
  //   glfwSewtFra(handle, &width, &height);
  // }

  // --- Visibility ---

  [[nodiscard]] bool is_visible() const { return glfwGetWindowAttrib(handle, GLFW_VISIBLE) == GLFW_TRUE; }

  void show() const { glfwShowWindow(handle); }

  void hide() const { glfwHideWindow(handle); }

  void show_decorations() const { glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE); }

  void hide_decorations() const { glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE); }

  // ---- Window Modes ----

  [[nodiscard]] bool is_full_screen() const { return glfwGetWindowMonitor(handle) != nullptr; }

  void set_full_screen(MonitorId const monitor_id = 0) const {
    auto opt_monitor = Monitor::from_id(monitor_id);
    if (not opt_monitor) {
      LOG_WARNING("Couldn't set window in full screen mode, monitor with id {} doesn't exist", monitor_id);
      return;
    }

    auto monitor = opt_monitor.value();
    monitor.attach_window(handle);
  }

  void windowed() const {
    auto res = size();
    glfwSetWindowMonitor(handle, nullptr, 0, 0, res.width, res.height, 0);
    glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE);
  }

  void borderless_full_screen(MonitorId const monitor_id = 0) const {
    auto opt_monitor = Monitor::from_id(monitor_id);
    if (not opt_monitor) {
      LOG_WARNING("Couldn't set window in borderless full screen mode, monitor with id {} doesn't exist", monitor_id);
      return;
    }

    auto monitor = opt_monitor.value();
    monitor.attach_window(handle);
    glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE);
  }

  [[nodiscard]] native_type native_handle() const noexcept { return handle; }

private:
  native_type handle {};
};

} // namespace rf3d
