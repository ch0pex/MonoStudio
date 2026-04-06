#pragma once

#include "reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp"

namespace rf3d::vk::detail {

/**
 * Buffer resource allocated on the GPU, containing both the buffer handle and the allocation information
 * this allocation is handled by VMA and should be freed using the allocator, not directly
 */
struct BufferAllocation {
  core::Buffer buffer_handle     = nullptr;
  Allocation allocation_handle   = nullptr;
  AllocationInfo allocation_info = {};
};

inline constexpr AllocationCreateInfo mapped_allocation_create_info = {
  .flags          = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
  .usage          = VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
  .requiredFlags  = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  .preferredFlags = {},
  .memoryTypeBits = 0,
  .pool           = nullptr,
  .pUserData      = nullptr,
  .priority       = 0.0F,
};

inline constexpr AllocationCreateInfo device_local_allocation_create_info = {
  .flags          = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
  .usage          = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
  .requiredFlags  = {},
  .preferredFlags = {},
  .memoryTypeBits = 0,
  .pool           = nullptr,
  .pUserData      = nullptr,
  .priority       = 0.0F,
};

} // namespace rf3d::vk::detail
