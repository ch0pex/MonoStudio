/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file viewport.hpp
 * @date 26/05/2026
 * @brief Viewport and 2D rectangle descriptor types
 */

#pragma once

// --- Includes ---
#include "reflect3d/math/math.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d {

struct Rect2D {
  math::uvec2 offset {0, 0};
  math::uvec2 extent {0, 0};
};

struct Viewport {
  Rect2D rect {};
  float min_depth {0.0F};
  float max_depth {1.0F};
};

} // namespace rf3d
