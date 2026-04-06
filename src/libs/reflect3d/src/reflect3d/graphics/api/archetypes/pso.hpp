#pragma once

#include "reflect3d/graphics/core/pso_states.hpp"

#include <cstdint>

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
