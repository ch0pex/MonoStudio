/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file texture_view.hpp
 * @date 30/03/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/texture_info.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---

namespace rf3d::vk {

using TextureView = TextureView<detail::core::Image, detail::core::ImageView>;

}
