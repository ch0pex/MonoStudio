#pragma once

#include "reflect3d/graphics/vk/memory/vk_dynamic_buffer.hpp"
#include "reflect3d/render/vertex.hpp"

namespace rf3d::gfx::vk {

struct DynamicVertexBuffer : DynamicBuffer<Vertex> {
  explicit DynamicVertexBuffer(std::size_t const count) :
    DynamicBuffer {core::BufferCreateInfo {
      .size        = count * sizeof(Vertex),
      .usage       = core::BufferUsageFlagBits::eVertexBuffer,
      .sharingMode = core::SharingMode::eExclusive,
    }} { }
};


} // namespace rf3d::gfx::vk
