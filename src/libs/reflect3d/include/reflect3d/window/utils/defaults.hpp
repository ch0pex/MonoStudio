#pragma once


#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_handle.hpp"

namespace rflect::defaults {

inline WindowHandle make_window_handle(rflect::config::Window const& config = {}) {
  return WindowBuilder(config).default_callbacks().build();
}

inline Window make_window(rflect::config::Window const& config = {}) { //
  return Window {make_window_handle(config)};
}

} // namespace rflect::defaults
