/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file compiler_options.hpp
 * @date 10/06/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstdint>
#include <string>
#include <vector>

// --- System ---


namespace rf3d::shader {

enum class DebugInfoLevel : std::uint8_t {
  none = 0,
  minimal,
  standard,
  maximal,
};

#ifndef NDEBUG
inline constexpr DebugInfoLevel default_debug_info_level = DebugInfoLevel::none;
#else
inline constexpr DebugInfoLevel default_debug_info_level = DebugInfoLevel::DebugInfoLevel;
#endif

struct CompilerOptions {
  std::vector<std::string> search_paths {};
  DebugInfoLevel debug_info_level = default_debug_info_level;
};

} // namespace rf3d::shader
