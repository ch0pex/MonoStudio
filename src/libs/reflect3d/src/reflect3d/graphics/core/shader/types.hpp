/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file types.hpp
 * @date 26/05/2026
 * @brief Shader bytecode, layout and stage type definitions
 */

#pragma once

// --- Includes ---

// --- Dependencies ---
#include <mono/containers/span.hpp>

// --- External dependencies ---
#include <slang.h>

// --- STD ---
#include <cstdint>
#include <string>

// --- System ---


namespace rf3d::shader {

using Bytecode = mono::span<char const>;
using Layout   = slang::ProgramLayout;

enum class Stage : std::uint8_t {
  vertex = 0,
  fragment,
  compute,
  tessellation_control,
  tessellation_evaluation,
  geometry,
};

inline std::string entry_point_name(Stage const stage) {
  switch (stage) {
    case Stage::vertex:
      return "vertMain";
    case Stage::fragment:
      return "fragMain";
    case Stage::compute:
      return "computeMain";
    case Stage::tessellation_control:
      return "tessControlMain";
    case Stage::tessellation_evaluation:
      return "tessEvalMain";
    case Stage::geometry:
      return "geometryMain";
    default:
      return "main";
  }
}

} // namespace rf3d::shader
