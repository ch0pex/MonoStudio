/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file pso.hpp
 * @date 26/05/2026
 * @brief PipelineState archetype for GPU pipeline state object modeling
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/pso_states.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstdint>

// --- System ---


namespace rf3d::archetypes {

/**
 * PipelineState archetype that models a GPU pipeline state object.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct PipelineState {
  using handle_type = std::uintptr_t;

  PipelineState() = default;

  PipelineState(PipelineState const&) = delete;

  PipelineState(PipelineState&&) = default;

  PipelineState& operator=(PipelineState const&) = delete;

  PipelineState& operator=(PipelineState&&) = default;

  ~PipelineState() = default;

  [[nodiscard]] PsoType type() const;

  [[nodiscard]] handle_type handle() const;
};

} // namespace rf3d::archetypes
