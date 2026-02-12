#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk {

/**
 * Buffer resource allocated on the GPU, containing both the buffer handle and the allocation information
 * this allocation is handled by VMA and should be freed using the allocator, not directly
 */
struct BufferAllocation {
  core::Buffer buffer_handle;
  Allocation allocation_handle;
  VmaAllocationInfo allocation_info;
};


} // namespace rf3d::gfx::vk
