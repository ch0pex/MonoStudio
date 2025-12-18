#pragma once

#include "mono/logging/logger_config.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_types.hpp"

//
#include <mono/config/base_config.hpp>

#include <rfl/toml/load.hpp>

namespace vix::config {

struct Vixel {
  using config_concept = mono::config::Tag;
  mono::config::Logger logger;
  rf3d::config::Window window;
};


} // namespace vix::config
//
template<>
vix::config::Vixel mono::config::example<vix::config::Vixel>() {
  return {
    .logger = {.name = "VixelLogger", .path = "/tmp/vixel/logger"},
    .window = {
      .title = "Vixel Game",
      .mode  = rf3d::WindowMode::windowed,
    }
  };
}
