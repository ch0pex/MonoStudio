/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file reflection.hpp
 * @date 07/05/2026
 * @brief Shader reflection utilities
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/detail/slang_format_map.hpp"
#include "reflect3d/graphics/core/pso_descriptor.hpp"
#include "reflect3d/graphics/core/shader/program.hpp"

// --- External dependencies ---
#include <slang.h>

// --- STD ---
#include <cstdint>
#include <string_view>
#include <vector>

// --- System ---

namespace rf3d::shader {

namespace detail {

inline bool is_varying_input(slang::VariableLayoutReflection* param) {

  if (param == nullptr) {
    throw std::invalid_argument("Parameter reflection cannot be null");
  }

  for (std::uint32_t c = 0; c < param->getCategoryCount(); ++c) {
    if (param->getCategoryByIndex(c) == slang::VaryingInput) {
      return true;
    }
  }
  return false;
}


inline VertexBufferBinding extract_vertex_buffer_binding(slang::TypeLayoutReflection* layout) {
  std::vector<VertexBindingAttribute> attributes;
  std::uint32_t cumulative_offset = 0;

  for (std::uint32_t j = 0; j < layout->getFieldCount(); ++j) {
    auto* field_layout = layout->getFieldByIndex(j);

    char const* semantic = field_layout->getSemanticName();
    if (semantic != nullptr and std::string_view(semantic).starts_with("SV_")) {
      continue;
    }

    Format const fmt = map_slang_format_to_rf3d(field_layout->getType());
    if (fmt == Format::unknown) {
      continue;
    }

    attributes.emplace_back(cumulative_offset, fmt);
    cumulative_offset += format_byte_size(fmt);
  }

  return {
    .byte_stride = cumulative_offset,
    .attributes  = std::move(attributes),
  };
}


} // namespace detail


/**
 * @brief Finds vertex buffer bindings from shader reflection data.
 *
 * This function looks for varying input
 * parameters in the vertex shader entry point(s) and extracts their field information to construct vertex buffer
 * bindings. It ignores fields with semantics starting with "SV_" (system values) and only considers fields with
 * recognized formats.
 *
 * @param program Shader program
 * @return Vertex buffer bindings extracted from the shader program reflection.
 */
inline std::vector<VertexBufferBinding> find_vertex_bindings(Program const& program) {
  std::vector<VertexBufferBinding> bindings;

  for (auto* entry_point: program.entry_points()) {
    if (entry_point->getStage() != SLANG_STAGE_VERTEX) {
      continue;
    }

    for (std::uint32_t i = 0; i < entry_point->getParameterCount(); ++i) {
      auto* param = entry_point->getParameterByIndex(i);

      if (not detail::is_varying_input(param))
        continue;

      auto* type        = param->getType();
      auto* type_layout = param->getTypeLayout();

      if (type->getKind() != slang::TypeReflection::Kind::Struct) {
        continue;
      }

      auto binding = detail::extract_vertex_buffer_binding(type_layout);
      if (not binding.attributes.empty()) {
        bindings.emplace_back(std::move(binding));
      }
    }
  }

  return bindings;
}

} // namespace rf3d::shader
