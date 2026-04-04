#pragma once

#include "reflect3d/graphics/vk2/detail/memory/vk_buffer_allocation.hpp"
#include "reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk2/detail/vk_instance.hpp"

namespace rf3d::vk::detail {

class MemoryAllocator {
public:
  using handle_type = Allocator;

  explicit MemoryAllocator(Allocator alloc) : allocator(alloc) { }

  ~MemoryAllocator() { vmaDestroyAllocator(allocator); }

  MemoryAllocator(MemoryAllocator const&) = default;

  MemoryAllocator(MemoryAllocator&&) = delete;

  MemoryAllocator& operator=(MemoryAllocator const&) = default;

  MemoryAllocator& operator=(MemoryAllocator&&) = delete;


  BufferAllocation
  allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info = {}) {
    VkBuffer buffer          = nullptr;
    VmaAllocation allocation = nullptr;
    VmaAllocationInfo allocation_info;

    VkBufferCreateInfo const vk_buffer_info = buf_info;
    vmaCreateBuffer(
        allocator, //
        &vk_buffer_info, //
        &alloc_info, //
        &buffer, //
        &allocation, //
        &allocation_info //
    );

    return {
      .buffer_handle     = buffer,
      .allocation_handle = allocation,
      .allocation_info   = allocation_info,
    };
  }

  void free_buffer(BufferAllocation const& buffer_allocation) {
    vmaDestroyBuffer(allocator, buffer_allocation.buffer_handle, buffer_allocation.allocation_handle);
  }


  [[nodiscard]] handle_type const& handle() const { return allocator; }

private:
  handle_type allocator;
};


} // namespace rf3d::vk::detail
