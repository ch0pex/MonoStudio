#pragma once

//
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window_types.hpp"

//
#include <GLFW/glfw3.h>

//
#ifdef MONO_USE_CPP_MODULES
#else
#include <string>
#include <unordered_map>
#endif

namespace rf3d::config {

// --- Window Types  ---

struct Window {
  std::string title {"Rflect"};
  Resolution resolution {.width = 1920, .height = 1080};
  WindowMode mode {WindowMode::windowed};
  MonitorId monitor {0};
  std::unordered_map<WindowHint, WindowHintValue> hints {
    {GLFW_RESIZABLE, GLFW_FALSE},
    {GLFW_CLIENT_API, GLFW_NO_API},
  };
};

} // namespace rf3d::config
