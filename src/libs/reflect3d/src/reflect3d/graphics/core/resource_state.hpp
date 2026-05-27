/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file resource_state.hpp
 * @date 26/05/2026
 * @brief GPU resource state enumeration for barrier transitions
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstdint>

// --- System ---


namespace rf3d {

enum class ResourceState : std::uint8_t {
  undefined = 0,
  vertex_buffer,
  constant_buffer,
  index_buffer,
  render_target,
  depth_write,
  depth_read,
  shader_resource,
  unordered_access,
  copy_source,
  copy_destination,
  indirect_argument,
  present
};

}
