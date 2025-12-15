#pragma once

#include "mono/execution/stop_token.hpp"
#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"

namespace vix {

// clang-format off
inline rf3d::Window main_window(rf3d::config::Window const& config = {}) {

  return rf3d::Window {  
    rf3d::WindowBuilder() 
      .on_close([](rf3d::WindowHandle::native_type) { 
        LOG_INFO("Closing window was requested by user");
      mono::ex::request_stop(); 
      })
      .build()
  }; // clang-format on
}

} // namespace vix
