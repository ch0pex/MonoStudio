#pragma once

#include "reflect3d/graphics/core2/pso_states.hpp"

#include <cstdint>

namespace rf3d::vk {

class PipelineState {
public:
  using handle_type = std::uintptr_t;

  PipelineState() = default;

  PipelineState(PipelineState const&) = delete;

  PipelineState(PipelineState&&) = default;

  PipelineState& operator=(PipelineState const&) = delete;

  PipelineState& operator=(PipelineState&&) = default;

  ~PipelineState() = default;

  [[nodiscard]] PsoType type() const;

  [[nodiscard]] handle_type handle() const;

private:
};

} // namespace rf3d::vk
