/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file parser_detail.hpp
 * @date 26/05/2026
 * @brief Configuration file parser implementation details
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


namespace mono::config::detail {

template<typename T>
concept HasLogger = requires(T config) {
  { config.logger };
};


} // namespace mono::config::detail
