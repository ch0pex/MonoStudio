/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_exception.hpp
 * @date 26/05/2026
 * @brief Vulkan-specific exception type
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <stdexcept>

// --- System ---


namespace rf3d::vk::detail {

struct Exception : std::runtime_error {
  using runtime_error::runtime_error;
};


} // namespace rf3d::vk::detail
