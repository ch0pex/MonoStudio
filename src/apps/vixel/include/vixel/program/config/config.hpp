#pragma once

#include "reflect3d/window/window_config.hpp"

//
#include <mono-core/config/base_config.hpp>

#include <rfl/toml/load.hpp>

namespace vix::config {

struct Vixel {
  rflect::config::Engine engine;
  rflect::config::Window window;
};

} // namespace vix::config
