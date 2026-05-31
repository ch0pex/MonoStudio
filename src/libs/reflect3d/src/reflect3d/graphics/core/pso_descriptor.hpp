/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file pso_descriptor.hpp
 * @date 06/04/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/format.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/core/shader/program.hpp"

// --- Dependencies ---
#include <mono/containers/span.hpp>

// --- External dependencies ---

// --- STD ---
#include <algorithm>
#include <string>
#include <vector>

// --- System ---

namespace rf3d {

using ShaderBytecode = mono::span<char const>;

struct VertexBindingAttribute {
  std::uint32_t offset {};
  Format format {};

  friend bool operator==(VertexBindingAttribute const& lhs, VertexBindingAttribute const& rhs) {
    return lhs.offset == rhs.offset and lhs.format == rhs.format;
  }
};

struct VertexBufferBinding {
  std::uint32_t byte_stride {};
  std::vector<VertexBindingAttribute> attributes {};

  friend bool operator==(VertexBufferBinding const& lhs, VertexBufferBinding const& rhs) {
    return lhs.byte_stride == rhs.byte_stride and std::ranges::equal(lhs.attributes, rhs.attributes);
  }
};

struct RenderPassLayout {
  std::vector<Format> color_attachments {};
  std::optional<Format> depth_attachment {};
  std::uint32_t sample_count {};
  std::uint32_t view_mask {};
};

enum class BindingType : uint8_t {
  constant_buffer,
  storage_buffer,
  rw_storage_buffer,
  texture_2D,
  rw_texture_2D,
  sampler_state,
};

struct BindingGroupLayout {
  std::uint32_t index {};
  BindingType type {};
  shader::Stage stage {};
};

/**
 * @brief Pipeline creation descriptor that contains all the necessary information to create a graphics pipeline state
 * object.
 *
 * This descriptor is designed to be backend-agnostic, containing only the information that is relevant for pipeline.
 * Bindings can be either be left empty for automatic extraction from shader reflection, or explicitly provided to
 * override the reflection results.
 */
struct PipelineCreateInfo {
  std::string debug_name {};
  shader::Program shader;
  RasterizerState rasterizer_state {};
  RenderPassLayout render_pass_layout {};

  // --- Reflected by default ---
  std::vector<VertexBufferBinding> vertex_buffer_bindings {};
  std::vector<BindingGroupLayout> binding_groups {};
};

} // namespace rf3d
