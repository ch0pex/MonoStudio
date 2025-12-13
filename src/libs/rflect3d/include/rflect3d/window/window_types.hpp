#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

namespace rflect {

using WindowHint      = int;
using WindowHintValue = int;
using NativeWindow    = GLFWwindow*;
using NativeMonitor   = GLFWmonitor*;
using MonitorId       = std::uint8_t;

enum class WindowMode : std::uint8_t {
  exclusive_full_screen = 0,
  borderless_full_screen,
  windowed_borderless,
  windowed,
};

} // namespace rflect
