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
#include <tuple>
#include <type_traits>
#include <utility>

// --- System ---

namespace mono::tuple {

template<typename Tuple>
concept empty = std::tuple_size_v<Tuple> == 0 and mono::meta::specialization_of<Tuple, std::tuple>;

template<typename Tuple, std::size_t N>
concept max_capacity = std::tuple_size_v<std::decay_t<Tuple>> <= N and //
                       mono::meta::specialization_of<std::decay_t<Tuple>, std::tuple>;

template<meta::specialization_of<std::tuple> Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
  std::apply(
      [&]<typename... Args>(Args&&... elements) { (f(std::forward<Args>(elements)), ...); }, //
      std::forward<Tuple>(t) //
  );
}

template<meta::specialization_of<std::tuple> Tuple, typename... Func>
void visit(Tuple&& t, Func&&... f) {
  auto apply_funcs_to_val = [&f...]<typename Arg>(Arg&& val) {
    (
        [&]<typename CurrentFunc>(CurrentFunc const& current_func) {
          if constexpr (std::is_invocable_v<CurrentFunc, Arg>) {
            current_func(std::forward<Arg>(val));
          }
        }(f),
        ...
    );
  };
  std::apply(
      [&]<typename... Args>(Args&&... elements) { (apply_funcs_to_val(std::forward<Args>(elements)), ...); },
      std::forward<Tuple>(t)
  );
}

template<meta::specialization_of<std::tuple> Tuple, typename Func>
auto transform(Tuple&& t, Func&& f) {
  constexpr auto n = std::tuple_size_v<std::decay_t<Tuple>>;
  return [&f, &t]<std::size_t... Idx>(std::index_sequence<Idx...>) {
    return std::make_tuple(f(std::get<Idx>(std::forward<Tuple>(t)))...);
  }(std::make_index_sequence<n> {});
}

} // namespace mono::tuple
