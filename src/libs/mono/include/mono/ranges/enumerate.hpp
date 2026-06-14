/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file enumerate.hpp
 * @date 16/06/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

#include <ranges>

// --- System ---

namespace mono::views {

#if __cpp_lib_ranges_enumerate >= 202302L

using std::views::enumerate;

#else

struct EnumerateFn : std::ranges::view_interface<EnumerateFn> {
  template<std::ranges::viewable_range R>
    requires std::ranges::forward_range<R>
  constexpr auto operator()(R&& r) const {
    return std::views::zip(std::views::iota(0), std::forward<R>(r));
  }

  template<std::ranges::forward_range R>
  friend constexpr auto operator|(R&& r, EnumerateFn fn) {
    return fn(std::forward<R>(r));
  }
};

inline constexpr EnumerateFn enumerate {};

#endif

} // namespace mono::views
