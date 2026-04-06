/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file load_store_operation.hpp
 * @date 31/03/2026
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

// --- System ---
namespace rf3d {

enum class LoadOperation : std::uint8_t {
  load,
  clear,
  discard,
};

enum class StoreOperation : std::uint8_t {
  store,
  discard,
};

} // namespace rf3d
