#pragma once

#include "reflect3d/graphics/vk/detail/memory/vk_dynamic_buffer.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_static_buffer.hpp"
#include "reflect3d/graphics/core/vertex.hpp"

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
