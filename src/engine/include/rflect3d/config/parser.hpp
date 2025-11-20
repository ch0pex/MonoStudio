#pragma once

#include "rflect3d/config/base_config.hpp"
#include "rflect3d/core/error/expected.hpp"
#include "rflect3d/core/logging/logger.hpp"

#include <rfl/toml/load.hpp>

namespace rflect::config {


/**
 * @brief Parses toml file to config structure
 */
template<Game GameConfig>
inline err::expected<GameConfig> parse_file(std::filesystem::path const& path) {
  auto const result = rfl::toml::load<GameConfig>(path);

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
  GlobalLogger::configure(config.logger);
  return {std::forward<GameConfig>(config)};
};

} // namespace rflect::config
