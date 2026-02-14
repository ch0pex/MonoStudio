#pragma once

#include "mono/meta/concepts.hpp"
#include "reflect3d/graphics/vk/memory/vk_dynamic_buffer.hpp"

namespace rf3d::gfx::vk {

template<mono::meta::trivially_copyable_value Type>
struct StagingBuffer : DynamicBuffer<Type> {
  explicit StagingBuffer(std::size_t const count) :
    DynamicBuffer<Type> {
      core::BufferCreateInfo {
        .size  = count * sizeof(Type),
        .usage = core::BufferUsageFlagBits::eTransferSrc,
      },
    } { }

  explicit StagingBuffer(std::span<Type const> elements) :
    DynamicBuffer<Type> {
      core::BufferCreateInfo {
        .size  = elements.size_bytes(),
        .usage = core::BufferUsageFlagBits::eTransferSrc,
      },
    } {
    this->insert_range(0, elements);
  }
};

} // namespace rf3d::gfx::vk
