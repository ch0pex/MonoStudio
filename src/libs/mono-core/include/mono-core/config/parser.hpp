#pragma once

#include "mono-core/config/base_config.hpp"
#include "mono-core/error/expected.hpp"
#include "mono-core/logging/logger.hpp"

#include <rfl/DefaultIfMissing.hpp>
#include <rfl/NoExtraFields.hpp>
#include <rfl/toml/load.hpp>

namespace rflect::config {


/**
 * @brief Parses toml file to config structure
 */
template<Game GameConfig>
err::expected<GameConfig> parse_file(std::filesystem::path const& path) {
  auto const result = rfl::toml::load<GameConfig, rfl::DefaultIfMissing, rfl::NoExtraFields>(path.string());

  if (not result) {
    return err::unexpected(result.error().what());
  }
  return {result.value()};
}

/**
 * @brief Initializes logger from game config
 */
template<Game GameConfig>
inline auto init_logger(GameConfig&& config) -> err::expected<GameConfig> {
  GlobalLogger::configure(config.engine.logger);
  return {std::forward<GameConfig>(config)};
};

} // namespace rflect::config
