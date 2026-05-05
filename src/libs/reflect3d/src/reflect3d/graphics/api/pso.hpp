#pragma once

#include "reflect3d/graphics/core/pso_states.hpp"

namespace rf3d {

template<typename T>
concept PipelineState = requires(T& pipeline) {
  typename T::handle_type;
  { pipeline.type() } -> std::convertible_to<PsoType>;
  { pipeline.handle() } -> std::convertible_to<typename T::handle_type const&>;
};

} // namespace rf3d
