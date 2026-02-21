#pragma once

#include <mono/config/base_config.hpp>
#include "mono/logging/logger_config.hpp"

namespace feed {

struct ProgramConfig {
  using config_concept = mono::config::Tag;
  mono::LoggerConfig logger;
  int feeds {0};
};

} // namespace feed
