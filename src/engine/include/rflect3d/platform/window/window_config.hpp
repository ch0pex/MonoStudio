#pragma once

//
#include "rflect3d/platform/window/utils/resolution.hpp"

//
#include <GLFW/glfw3.h>

//
#include <string>
#include <unordered_map>

namespace rflect {

using WindowHint = int;
using WindowHintValue = int;

struct WindowConfig {
  std::string title{"Rflect"};
  Resolution resolution{};
  std::unordered_map<WindowHint, WindowHintValue> hints{
      {GLFW_RESIZABLE, GLFW_FALSE},
      {GLFW_CLIENT_API, GLFW_NO_API},
  };
};

} // namespace rflect
