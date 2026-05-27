/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file log_error.hpp
 * @date 26/05/2026
 * @brief Error logging utilities for expected types
 */

#pragma once

// --- Includes ---
#include "mono/error/expected.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <print>
#include <stdexcept>

// --- System ---


namespace mono {

template<typename T>
inline constexpr auto print_error = [](std::runtime_error const& error) -> mono::expected<T> {
  std::println("\033[31m{}\033[0m", error.what()); // 31 = red, 0 = reset
  return mono::unexpected(error.what());
};

} // namespace mono
