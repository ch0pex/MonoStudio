/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file unordered_flat_map.hpp
 * @date 26/05/2026
 * @brief Unordered flat map container adapter (Boost.Unordered wrapper)
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <boost/unordered/unordered_flat_map.hpp>

// --- STD ---

// --- System ---


namespace mono {

template<
    typename K,
    typename V, //
    typename Hash      = std::hash<K>, //
    typename Pred      = std::equal_to<K>, //
    typename Allocator = std::allocator<std::pair<K const, V>> //
    > //
using unordered_flat_map = boost::unordered_flat_map<K, V, Hash, Pred, Allocator>;

}
