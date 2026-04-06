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

// --- Dependencies ---
#include <mono/containers/span.hpp>

// --- External dependencies ---

// --- STD ---
#include <string>

// --- System ---

namespace rf3d {

using ShaderBytecode = mono::span<char const>;

struct Shader {
  std::string entry_point {"main"};
  ShaderBytecode bytecode {};
};

struct BindingAttribute {
  std::uint32_t offset {};
  Format format {};
};

struct VertexBufferBinding {
  std::uint32_t byte_stride {};
  mono::span<BindingAttribute const> attributes {};
};

struct PipelineCreateInfo {
  std::string debug_name {};
  Shader vertex_shader {};
  Shader fragment_shader {};
  RasterizerState rasterizer_state {};
  mono::span<VertexBufferBinding const> vertex_buffer_bindings {};
};

} // namespace rf3d
