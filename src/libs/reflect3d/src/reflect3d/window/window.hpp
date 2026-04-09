#pragma once

#include "reflect3d/window/monitor.hpp"
#include "reflect3d/window/utils/callbacks.hpp"
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

  using handle_type     = GLFWwindow*;
  using position_type   = glm::u16vec2;
  using resolution_type = Resolution;

  /**********************
   *    Constructors    *
   **********************/

  Window(handle_type handle, mono::PassKey<WindowBuilder> /*unused*/) : window_handle(handle) { }

  Window(Window const& other) = delete;

  Window& operator=(Window const& other) = delete;

  Window(Window&& other) noexcept : window_handle(other.window_handle) { other.window_handle = nullptr; }

  Window& operator=(Window&& other) noexcept {
    if (&other != this) {
      std::swap(window_handle, other.window_handle);
    }
    return *this;
  }

  ~Window() {
    if (window_handle != nullptr) {
      // LOG_INFO("Destroying Window");
      glfwDestroyWindow(window_handle);
    }
  }

  /***************************
   *     Member functions    *
   ***************************/

  [[nodiscard]] std::string_view title() const { return {glfwGetWindowTitle(window_handle)}; }

  void title(std::string const& name) const { glfwSetWindowTitle(window_handle, name.c_str()); }

  [[nodiscard]] bool should_close() const { return glfwWindowShouldClose(window_handle) != 0; }

  void swap_buffers() const { glfwSwapBuffers(window_handle); }

  // --- Position - Size - Resolution ---

  void position(position_type const position) const {
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      LOG_WARNING("Wayland backend detected, setting window position is not supported");
      return;
    }
    glfwSetWindowPos(window_handle, position.x, position.y);
  }

  [[nodiscard]] position_type position() const {
    int x = 0;
    int y = 0;
    glfwGetWindowPos(window_handle, &x, &y);
    return {static_cast<std::uint16_t>(x), static_cast<std::uint16_t>(y)};
  }

  [[nodiscard]] resolution_type size() const {
    int width  = 0;
    int height = 0;
    glfwGetWindowSize(window_handle, &width, &height);
    return {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)};
  }

  void resize(resolution_type const size) const { glfwSetWindowSize(window_handle, size.width, size.height); }

  [[nodiscard]] resolution_type resolution() const {
    int width  = 0;
    int height = 0;
    glfwGetFramebufferSize(window_handle, &width, &height);
    return {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)};
  }

  // --- Surface ---

  void make_context_current() const { glfwMakeContextCurrent(window_handle); }


  // void resolution(resolution_type const resolution) const {
  //   int width  = 0;
  //   int height = 0;
  //   glfwSewtFra(handle, &width, &height);
  // }

  // --- Visibility ---

  [[nodiscard]] bool is_visible() const { return glfwGetWindowAttrib(window_handle, GLFW_VISIBLE) == GLFW_TRUE; }

  void show() const { glfwShowWindow(window_handle); }

  void hide() const { glfwHideWindow(window_handle); }

  void show_decorations() const { glfwSetWindowAttrib(window_handle, GLFW_DECORATED, GLFW_TRUE); }

  void hide_decorations() const { glfwSetWindowAttrib(window_handle, GLFW_DECORATED, GLFW_FALSE); }

  // ---- Window Modes ----

  [[nodiscard]] bool is_full_screen() const { return glfwGetWindowMonitor(window_handle) != nullptr; }

  void set_full_screen(MonitorId const monitor_id = 0) const {
    auto opt_monitor = Monitor::from_id(monitor_id);
    if (not opt_monitor) {
      LOG_WARNING("Couldn't set window in full screen mode, monitor with id {} doesn't exist", monitor_id);
      return;
    }

    auto monitor = opt_monitor.value();
    monitor.attach_window(window_handle);
  }

  void windowed() const {
    auto res = size();
    glfwSetWindowMonitor(window_handle, nullptr, 0, 0, res.width, res.height, 0);
    glfwSetWindowAttrib(window_handle, GLFW_DECORATED, GLFW_TRUE);
  }

  void borderless_full_screen(MonitorId const monitor_id = 0) const {
    auto opt_monitor = Monitor::from_id(monitor_id);
    if (not opt_monitor) {
      LOG_WARNING("Couldn't set window in borderless full screen mode, monitor with id {} doesn't exist", monitor_id);
      return;
    }

    auto monitor = opt_monitor.value();
    monitor.attach_window(window_handle);
    glfwSetWindowAttrib(window_handle, GLFW_DECORATED, GLFW_FALSE);
  }

  template<callbacks::WindowEvent Event>
  void set_callback(auto callback) const {
    callbacks::set_callback<Event>(window_handle, std::move(callback));
  }

  [[nodiscard]] handle_type handle() const noexcept { return window_handle; }

private:
  handle_type window_handle {};
};

} // namespace rf3d
