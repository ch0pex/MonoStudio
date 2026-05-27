/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file expected.hpp
 * @date 26/05/2026
 * @brief Expected type with error handling and formatting utilities
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <expected>
#include <format>
#include <stdexcept>

// --- System ---


namespace mono {

template<typename T, typename Error = std::runtime_error>
using expected = std::expected<T, Error>;

template<typename... Args>
inline std::unexpected<std::runtime_error> unexpected(std::string_view message, Args const&... args) {
  if constexpr (sizeof...(Args) == 0) {
    return std::unexpected<std::runtime_error> {std::runtime_error(std::string(message))};
  }

  return std::unexpected<std::runtime_error> {
    std::runtime_error(std::vformat(message, std::make_format_args(args...)))
  };
}

} // namespace mono
