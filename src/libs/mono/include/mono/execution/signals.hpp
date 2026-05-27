/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file signals.hpp
 * @date 26/05/2026
 * @brief Execution signal utilities
 */

#pragma once

// --- Includes ---
#include "mono/execution/stop_token.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace mono::ex {

/**
 * @brief Basic signaling setup
 *
 * Setups program signals to request a stop over a stop source and
 * returns the token of such source.
 */
inline std::stop_token setup_signals() {
  std::ignore = std::signal(SIGINT, request_stop);
  std::ignore = std::signal(SIGTERM, request_stop);

  return detail::stop_source.get_token();
}

} // namespace mono::ex
