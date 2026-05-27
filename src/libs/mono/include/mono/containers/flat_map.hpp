/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file flat_map.hpp
 * @date 26/05/2026
 * @brief Flat map container adapter (Boost.Container wrapper)
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <boost/container/flat_map.hpp>

// --- STD ---

// --- System ---


namespace mono {

template<
    typename K, //
    typename V, //
    typename Compare   = std::less<K>, //
    typename Allocator = std::allocator<std::pair<K const, V>> //
    >
using flat_map = boost::container::flat_map<K, V, Compare, Allocator>;

}
