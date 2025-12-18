#pragma once

#include "mono/config/base_config.hpp"
#include "mono/error/expected.hpp"
#include "mono/logging/logger.hpp"

#include <rfl/DefaultIfMissing.hpp>
#include <rfl/NoExtraFields.hpp>
#include <rfl/toml/load.hpp>

namespace mono::config::detail {

template<typename T>
concept HasLogger = requires(T config) {
  { config.logger };
};


} // namespace mono::config::detail
