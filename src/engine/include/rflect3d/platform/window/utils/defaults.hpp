#pragma once


#include "rflect3d/platform/window/window.hpp"
#include "rflect3d/platform/window/window_builder.hpp"
#include "rflect3d/platform/window/window_config.hpp"
#include "rflect3d/platform/window/window_handle.hpp"

namespace rflect::defaults {

inline WindowHandle make_window_handle(rflect::WindowConfig const& config = {}) {
  return WindowBuilder(config).default_callbacks().build();
}

inline Window make_window(rflect::WindowConfig const& config = {}) { //
  return Window {make_window_handle(config)};
}

} // namespace rflect::defaults
