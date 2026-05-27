/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file enum_flags.hpp
 * @date 26/05/2026
 * @brief Compile-time enum flag containment check utilities
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <type_traits>
#include <utility>

// --- System ---


namespace rf3d::detail {

template<typename T>
  requires(std::is_scoped_enum_v<T>)
consteval bool contains_flag(T const mask, T const flag) {
  return (std::to_underlying(mask) & std::to_underlying(flag)) != 0;
}

template<auto Mask, auto Flag>
concept ContainsFlag = detail::contains_flag(Mask, Flag);

} // namespace rf3d::detail
