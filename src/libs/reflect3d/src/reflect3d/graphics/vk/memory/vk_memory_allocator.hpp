#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk {

struct MemoryAllocator {
  using handle_type = VmaAllocator;

  explicit MemoryAllocator(handle_type vma_handle) : handle(vma_handle) { }

  ~MemoryAllocator() { vmaDestroyAllocator(handle); }

  MemoryAllocator(MemoryAllocator const&) = default;

  MemoryAllocator(MemoryAllocator&&) = delete;

  MemoryAllocator& operator=(MemoryAllocator const&) = default;

  MemoryAllocator& operator=(MemoryAllocator&&) = delete;

  handle_type handle;
};


} // namespace rf3d::gfx::vk
