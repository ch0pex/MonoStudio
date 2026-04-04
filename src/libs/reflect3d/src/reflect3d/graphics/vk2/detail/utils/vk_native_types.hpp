#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

namespace rf3d::vk::detail {

namespace core = ::vk;
namespace raii = ::vk::raii;

using Allocator            = VmaAllocator;
using Allocation           = VmaAllocation;
using AllocationInfo       = VmaAllocationInfo;
using AllocationCreateInfo = VmaAllocationCreateInfo;

} // namespace rf3d::vk::detail
