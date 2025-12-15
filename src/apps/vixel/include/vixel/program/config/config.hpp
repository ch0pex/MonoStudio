#pragma once

#include "reflect3d/window/window_config.hpp"

//
#include <mono/config/base_config.hpp>

#include <rfl/toml/load.hpp>

namespace vix::config {

struct Vixel {
  mono::config::Engine engine;
  rf3d::config::Window window;
};

} // namespace vix::config
