#pragma once

#include "mono/config/base_config.hpp"
#include "mono/error/expected.hpp"
#include "mono/logging/logger.hpp"

#include <rfl/DefaultIfMissing.hpp>
#include <rfl/NoExtraFields.hpp>
#include <rfl/toml/load.hpp>

namespace mono::config {

namespace detail {

template<typename T>
concept HasLogger = requires(T config) {
  { config.logger };
};


} // namespace detail


/**
 * @brief Parses toml file to config structure
 */
template<Program ProgramConfig>
err::expected<ProgramConfig> parse_file(std::filesystem::path const& path) {
  auto const result = rfl::toml::load<ProgramConfig, rfl::DefaultIfMissing, rfl::NoExtraFields>(path.string());

  if (not result) {
    return err::unexpected(result.error().what());
  }
  return {result.value()};
}

/**
 * @brief Initializes logger from game config
 */
template<Program ProgramConfig>
  requires(detail::HasLogger<ProgramConfig>)
inline void init_logger(ProgramConfig const& config) {
  GlobalLogger::configure(config.logger);
};

} // namespace mono::config
