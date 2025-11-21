#pragma once

#include "rflect3d/program/config/config.hpp"

#include <rfl/toml/load.hpp>

namespace vix::config {

inline Vixel program_example() {
  return {
    .logger = {
      .name = "Vixel",
      .path = "/tmp/", // TODO normalize paths /
    }
  };
}

} // namespace vix::config
