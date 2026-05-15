/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file slang_format_map.hpp
 * @date 13/05/2026
 * @brief Slang type reflection to rf3d Format mapping utilities
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/format.hpp"

// --- External dependencies ---
#include <slang.h>

// --- STD ---

namespace rf3d {

namespace detail {

// clang-format off
inline Format map_float32(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r32_sfloat;
    case 2:  return Format::rg32_sfloat;
    case 3:  return Format::rgb32_sfloat;
    case 4:  return Format::rgba32_sfloat;
    default: return Format::unknown;
  }
}

inline Format map_float16(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r16_sfloat;
    case 2:  return Format::rg16_sfloat;
    case 3:  return Format::rgb16_sfloat;
    case 4:  return Format::rgba16_sfloat;
    default: return Format::unknown;
  }
}

inline Format map_uint32(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r32_uint;
    case 2:  return Format::rg32_uint;
    case 3:  return Format::rgb32_uint;
    case 4:  return Format::rgba32_uint;
    default: return Format::unknown;
  }
}

inline Format map_sint32(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r32_sint;
    case 2:  return Format::rg32_sint;
    case 3:  return Format::rgb32_sint;
    case 4:  return Format::rgba32_sint;
    default: return Format::unknown;
  }
}

inline Format map_uint16(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r16_uint;
    case 2:  return Format::rg16_uint;
    case 4:  return Format::rgba16_uint;
    default: return Format::unknown;
  }
}

inline Format map_sint16(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r16_sint;
    case 2:  return Format::rg16_sint;
    case 4:  return Format::rgba16_sint;
    default: return Format::unknown;
  }
}

inline Format map_uint8(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r8_uint;
    case 2:  return Format::rg8_uint;
    case 4:  return Format::rgba8_unorm;
    default: return Format::unknown;
  }
}

inline Format map_sint8(std::size_t const components) {
  switch (components) {
    case 1:  return Format::r8_sint;
    case 2:  return Format::rg8_sint;
    default: return Format::unknown;
  }
}

} // namespace detail

inline Format map_slang_format_to_rf3d(slang::TypeReflection* const type) {
  using Kind   = slang::TypeReflection::Kind;
  using Scalar = slang::TypeReflection::ScalarType;

  slang::TypeReflection* scalar_type = type;
  std::size_t components             = 1;

  if (type->getKind() == Kind::Vector) {
    scalar_type = type->getElementType();
    components  = type->getElementCount();
  }
  else if (type->getKind() != Kind::Scalar) {
    return Format::unknown;
  }

  switch (scalar_type->getScalarType()) {
    case Scalar::Float32: return detail::map_float32(components);
    case Scalar::Float16: return detail::map_float16(components);
    case Scalar::UInt32:  return detail::map_uint32(components);
    case Scalar::Int32:   return detail::map_sint32(components);
    case Scalar::UInt16:  return detail::map_uint16(components);
    case Scalar::Int16:   return detail::map_sint16(components);
    case Scalar::UInt8:   return detail::map_uint8(components);
    case Scalar::Int8:    return detail::map_sint8(components);
    default:              return Format::unknown;
  }
}
// clang-format on

} // namespace rf3d
