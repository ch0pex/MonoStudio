#pragma once


#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"

namespace rf3d::defaults {

inline Window make_window_handle(config::Window const& config = {}) {
  return WindowBuilder(config).default_callbacks().build();
}

inline Window make_window(config::Window const& config = {}) { //
  return Window {make_window_handle(config)};
}

} // namespace rf3d::defaults
