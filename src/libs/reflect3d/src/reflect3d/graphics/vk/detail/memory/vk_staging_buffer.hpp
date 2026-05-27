/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_staging_buffer.hpp
 * @date 26/05/2026
 * @brief Vulkan staging buffer for CPU-to-GPU data transfers
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/memory/vk_dynamic_buffer.hpp"

// --- Dependencies ---
#include <mono/meta/concepts.hpp>

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

template<mono::meta::trivially_copyable_value Type>
struct StagingBuffer : DynamicBuffer<Type> {
  explicit StagingBuffer(std::size_t const count) :
    DynamicBuffer<Type> {
      core::BufferCreateInfo {
        .size  = count * sizeof(Type),
        .usage = core::BufferUsageFlagBits::eTransferSrc,
      },
    } { }

  explicit StagingBuffer(mono::span<Type const> elements) :
    DynamicBuffer<Type> {
      core::BufferCreateInfo {
        .size  = elements.size_bytes(),
        .usage = core::BufferUsageFlagBits::eTransferSrc,
      },
    } {
    this->insert_range(0, elements);
  }
};

} // namespace rf3d::vk::detail
