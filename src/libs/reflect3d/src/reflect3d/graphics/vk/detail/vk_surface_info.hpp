/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_surface_info.hpp
 * @date 26/05/2026
 * @brief Vulkan surface capabilities, formats and present modes aggregation
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

struct SurfaceInfo {
  core::SurfaceCapabilitiesKHR capabilities;
  std::vector<core::SurfaceFormatKHR> formats;
  std::vector<core::PresentModeKHR> present_modes;
};

} // namespace rf3d::vk::detail
