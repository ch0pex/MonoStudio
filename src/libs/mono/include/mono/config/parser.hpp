/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file parser.hpp
 * @date 26/05/2026
 * @brief TOML configuration file parser
 */

#pragma once

// --- Includes ---
#include "mono/config/base_config.hpp"
#include "mono/error/expected.hpp"
#include "mono/logging/logger.hpp"

// --- Dependencies ---

// --- External dependencies ---
#include <rfl/DefaultIfMissing.hpp>
#include <rfl/NoExtraFields.hpp>
#include <rfl/toml/load.hpp>

// --- STD ---

// --- System ---


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
expected<ProgramConfig> parse_file(std::filesystem::path const& path) {
  auto path_str     = path.string();
  auto const result = rfl::toml::load<ProgramConfig, rfl::DefaultIfMissing, rfl::NoExtraFields>(path_str);

  if (not result) {
    return unexpected(result.error().what());
  }
  return {result.value()};
}

/**
 * @brief Initializes logger from game config
 */
template<Program ProgramConfig>
  requires(detail::HasLogger<ProgramConfig>)
inline void init_logger([[maybe_unused]] ProgramConfig const& config) {
  // GlobalLogger::configure(config.logger); TODO: change this
};

} // namespace mono::config
