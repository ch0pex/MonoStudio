/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file command_list_type.hpp
 * @date 26/05/2026
 * @brief Command list type enumeration for copy, compute and graphics queues
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstdint>

// --- System ---


namespace rf3d {

enum CommandListType : std::uint8_t {
  copy     = 1 << 0,
  compute  = 1 << 1,
  graphics = 1 << 2,
};

} // namespace rf3d
