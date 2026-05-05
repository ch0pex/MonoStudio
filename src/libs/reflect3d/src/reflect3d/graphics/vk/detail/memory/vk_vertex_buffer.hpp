#pragma once

#include "reflect3d/graphics/vk/detail/memory/vk_dynamic_buffer.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_static_buffer.hpp"
#include "reflect3d/graphics/core/vertex.hpp"

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
