#pragma once

#include "rflect3d/core/misc/passkey.hpp"
#include "rflect3d/platform/window/utils/context.hpp"
#include "rflect3d/platform/window/utils/resolution.hpp"

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

private:
  native_type handle {};
};

} // namespace rflect
