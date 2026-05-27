/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_barriers.hpp
 * @date 26/05/2026
 * @brief Vulkan pipeline barrier info structure definition
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

struct BarrierInfo {
  core::PipelineStageFlags2 stage_mask;
  core::AccessFlags2 access_mask;
  core::ImageLayout image_layout;
};

} // namespace rf3d::vk::detail
