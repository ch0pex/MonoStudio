#pragma once

#include "reflect3d/graphics/vk/memory/vk_buffer.hpp"
#include "reflect3d/graphics/vk/memory/vk_staging_buffer.hpp"


namespace rf3d::gfx::vk {

template<mono::meta::trivially_copyable_value Type>
struct StaticBufferCreateInfo {
  mono::span<Type const> data;
  core::BufferUsageFlags usage;
};


template<mono::meta::trivially_copyable_value Type>
struct StaticBuffer : Buffer<Type> {
  explicit StaticBuffer(StaticBufferCreateInfo<Type> const& create_info) :
    Buffer<Type> {
      core::BufferCreateInfo {
        .size  = create_info.data.size_bytes(),
        .usage = core::BufferUsageFlagBits::eTransferDst | create_info.usage,
      },
      device_local_allocation_create_info
    } {
    StagingBuffer<Type> staging_buffer {create_info.data};
    gpu::upload_buffer(
        this->handle(), staging_buffer.handle(), core::BufferCopy {.size = create_info.data.size_bytes()}
    );
  }
};

} // namespace rf3d::gfx::vk
