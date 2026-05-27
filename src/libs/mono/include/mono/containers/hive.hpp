/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file hive.hpp
 * @date 26/05/2026
 * @brief Hive (stable-pointer unordered collection) container adapter
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#include <plf_colony.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

// --- STD ---

// --- System ---


namespace mono {

template<class T, class Alloc = std::allocator<T>, plf::priority priority = plf::performance>
using hive = plf::colony<T, Alloc, priority>;

} // namespace mono
