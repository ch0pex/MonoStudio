/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_index_buffer.hpp
 * @date 26/05/2026
 * @brief Vulkan index buffer wrappers (static and dynamic)
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

struct IndexBuffer : StaticBuffer<Index> {
  explicit IndexBuffer(mono::span<Index const> indices) :
    StaticBuffer {
      StaticBufferCreateInfo {
        .data  = indices,
        .usage = core::BufferUsageFlagBits::eIndexBuffer,
      },
    } { }
};

struct DynamicIndexBuffer : DynamicBuffer<Index> {
  explicit DynamicIndexBuffer(std::size_t const count) :
    DynamicBuffer {
      core::BufferCreateInfo {
        .size  = count * sizeof(Index),
        .usage = core::BufferUsageFlagBits::eIndexBuffer,
      },
    } { }

  explicit DynamicIndexBuffer(mono::span<Index const> indices) :
    DynamicBuffer {
      core::BufferCreateInfo {
        .size  = indices.size_bytes(),
        .usage = core::BufferUsageFlagBits::eIndexBuffer,
      },
    } {
    insert_range(0, indices);
  }
};


} // namespace rf3d::vk::detail
