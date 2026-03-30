/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file tuple.hpp
 * @date 30/03/2026
 * @brief Tuple utilities
 *
 */

#pragma once

// --- Includes ---
#include "mono/meta/concepts.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstddef>
#include <tuple>
#include <utility>

// --- System ---

namespace mono::tuple {

template<typename Tuple>
concept empty = std::tuple_size_v<Tuple> == 0 and mono::meta::specialization_of<Tuple, std::tuple>;

template<typename Tuple, std::size_t N>
concept max_capacity = std::tuple_size_v<Tuple> <= N and mono::meta::specialization_of<Tuple, std::tuple>;

template<mono::meta::specialization_of<std::tuple> Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
  constexpr auto n = std::tuple_size_v<std::decay_t<Tuple>>;
  [&f, &t]<std::size_t... Idx>(std::index_sequence<Idx...>) {
    (f(std::get<Idx>(std::forward<Tuple>(t))), ...);
  }(std::make_index_sequence<n> {});
}

template<mono::meta::specialization_of<std::tuple> Tuple, typename Func>
auto transform(Tuple&& t, Func&& f) {
  constexpr auto n = std::tuple_size_v<std::decay_t<Tuple>>;
  return [&f, &t]<std::size_t... Idx>(std::index_sequence<Idx...>) {
    return std::make_tuple(f(std::get<Idx>(std::forward<Tuple>(t)))...);
  }(std::make_index_sequence<n> {});
}

} // namespace mono::tuple
