/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file pso.hpp
 * @date 26/05/2026
 * @brief PipelineState concept for GPU pipeline state object constraints
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/pso_states.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d {

template<typename T>
concept PipelineState = requires(T& pipeline) {
  typename T::handle_type;
  { pipeline.type() } -> std::convertible_to<PsoType>;
  { pipeline.handle() } -> std::convertible_to<typename T::handle_type const&>;
};

} // namespace rf3d
