#pragma once

#include <mono/config/base_config.hpp>

namespace feed {

struct ProgramConfig {
  using config_concept = mono::config::Tag;
  int feeds;
};

} // namespace feed
