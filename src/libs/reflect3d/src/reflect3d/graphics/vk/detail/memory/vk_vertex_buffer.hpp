/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_vertex_buffer.hpp
 * @date 26/05/2026
 * @brief Vulkan vertex buffer wrappers (static and dynamic)
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/vertex.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_dynamic_buffer.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_static_buffer.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

struct VertexBuffer : StaticBuffer<Vertex> {
  explicit VertexBuffer(mono::span<Vertex const> vertices) :
    StaticBuffer {
      StaticBufferCreateInfo {
        .data  = vertices,
        .usage = core::BufferUsageFlagBits::eVertexBuffer,
      },
    } { }
};

struct DynamicVertexBuffer : DynamicBuffer<Vertex> {
  explicit DynamicVertexBuffer(std::size_t const count) :
    DynamicBuffer {
      core::BufferCreateInfo {
        .size  = count * sizeof(Vertex),
        .usage = core::BufferUsageFlagBits::eVertexBuffer,
      },
    } { }

  explicit DynamicVertexBuffer(mono::span<Vertex const> vertices) :
    DynamicBuffer {
      core::BufferCreateInfo {
        .size  = vertices.size_bytes(),
        .usage = core::BufferUsageFlagBits::eVertexBuffer,
      },
    } {
    insert_range(0, vertices);
  }
};


} // namespace rf3d::vk::detail
