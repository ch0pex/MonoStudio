/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file stable_vector.hpp
 * @date 26/05/2026
 * @brief Stable vector container adapter (Boost.Container wrapper)
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <boost/container/stable_vector.hpp>

// --- STD ---

// --- System ---


namespace mono {

template<typename T>
using stable_vector = boost::container::stable_vector<T>;

}
