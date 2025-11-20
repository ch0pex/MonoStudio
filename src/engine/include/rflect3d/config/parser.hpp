#pragma once

#include "rflect3d/core/error/expected.hpp"
#include "rflect3d/core/logging/logger.hpp"
#include "rflect3d/core/logging/logger_config.hpp"

#include <rfl/toml/load.hpp>

namespace rflect::config {

/**
 * @brief Parses toml file to config structure
 */
template <typename T>
inline err::expected<T> parse_file(std::filesystem::path const &path) {
  auto const result = rfl::toml::load<T>(path);

  if (not result) {
    return err::unexpected(result.error().what());
  }
  return {result.value()};
}

/**
 * @brief Initializes logger from game config
 */
template <typename T>
inline auto init_logger = [](T &&config) -> err::expected<T> {
  GlobalLogger::configure(config.logger);
  return {std::move(config)};
};

} // namespace rflect::config
