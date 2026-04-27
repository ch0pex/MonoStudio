/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file draw_stream.hpp
 * @date 30/03/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/renderpass_descriptor.hpp"
#include "reflect3d/graphics/vk/buffer.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/pso.hpp"

// --- Dependencies ---
#include <mono/containers/span.hpp>

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk {

using DrawCall = DrawCall<PipelineState, VertexBuffer, IndexBuffer>;

template<typename DrawCallType = DrawCall>
class DrawStreamBuilder {
public:
  using draw_type = DrawCallType;

  DrawStreamBuilder() = default;

  void draw(draw_type const& draw_call) { vertex_data.push_back(draw_call); }

  [[nodiscard]] mono::span<draw_type const> get_stream() const { return vertex_data; }

private:
  std::vector<draw_type> vertex_data;
};

// --- Deduction guides ---
DrawStreamBuilder() -> DrawStreamBuilder<>;

} // namespace rf3d::vk
