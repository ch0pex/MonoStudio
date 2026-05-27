/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file static_vector.hpp
 * @date 26/05/2026
 * @brief Static vector container adapter (Boost.Container wrapper)
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <boost/container/static_vector.hpp>

// --- STD ---

// --- System ---


namespace mono {

template<typename T, std::size_t N>
using static_vector = boost::container::static_vector<T, N>;

}
