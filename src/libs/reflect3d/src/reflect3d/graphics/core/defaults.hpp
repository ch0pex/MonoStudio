/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file defaults.hpp
 * @date 26/05/2026
 * @brief Default rendering constants including frame count and timeout values
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <chrono>

// --- System ---


namespace rf3d::defaults {

inline constexpr std::uint32_t max_frames_in_flight = 2U;

inline constexpr std::chrono::nanoseconds wait_timeout = std::chrono::milliseconds {5000};

} // namespace rf3d::defaults
