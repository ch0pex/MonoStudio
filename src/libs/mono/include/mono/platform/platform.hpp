/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file platform.hpp
 * @date 26/05/2026
 * @brief Platform initialization and detection utilities
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <string_view>

// --- System ---


namespace mono::os {

#ifdef WIN32

inline std::string_view constexpr name = "windows";

#else

inline std::string_view constexpr name = "linux";

#endif

} // namespace mono::os
