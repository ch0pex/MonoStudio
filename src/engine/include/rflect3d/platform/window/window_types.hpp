#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

namespace rflect {

using WindowHint      = int;
using WindowHintValue = int;
using NativeWindow    = GLFWwindow*;
using Monitor         = GLFWmonitor*;

enum class WindowMode : std::uint8_t {
  full_screen = 0,
  full_screen_borderless,
  windowed_borderless,
  windowed,
};

} // namespace rflect
