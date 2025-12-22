#pragma once

//
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window_types.hpp"

//
#include <GLFW/glfw3.h>

//
#include <cstdint>
#include <string>
#include <unordered_map>

namespace rf3d::config {

// --- Window Types  ---

struct Window {
  std::string title {"Rflect"};
  Resolution resolution {.width = 1920, .height = 1080};
  WindowMode mode {WindowMode::windowed};
  std::uint8_t monitor {};
  std::unordered_map<WindowHint, WindowHintValue> hints {{
    // {GLFW_RESIZABLE, GLFW_FALSE},
    {GLFW_CLIENT_API, GLFW_NO_API},
  }};
};

} // namespace rf3d::config
