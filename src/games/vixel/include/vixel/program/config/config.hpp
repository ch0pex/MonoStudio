#pragma once

#include "rflect3d/config/base_config.hpp"
#include "rflect3d/platform/window/utils/resolution.hpp"
#include "rflect3d/platform/window/window_config.hpp"
#include "rflect3d/platform/window/window_types.hpp"

#include <rfl/toml/load.hpp>

namespace vix::config {

struct Vixel {
  rflect::config::Engine engine;
  rflect::config::Window window;
};

} // namespace vix::config
