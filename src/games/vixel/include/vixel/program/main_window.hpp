#pragma once

#include "rflect3d/core/execution/stop_token.hpp"
#include "rflect3d/window/window.hpp"
#include "rflect3d/window/window_builder.hpp"
#include "rflect3d/window/window_config.hpp"

namespace vix {

// clang-format off
inline rflect::Window main_window(rflect::config::Window const& config = {}) {

  return rflect::Window {  
    rflect::WindowBuilder() 
      .on_close([](rflect::WindowHandle::native_type) { 
        LOG_INFO("Closing window was requested by user");
      rflect::ex::request_stop(); 
      })
      .build()
  }; // clang-format on
}

} // namespace vix
