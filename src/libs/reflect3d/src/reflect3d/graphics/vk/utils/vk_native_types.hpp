#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

namespace rf3d::gfx::vk {

namespace core = ::vk;
namespace raii = ::vk::raii;

using FrameIndex           = std::uint32_t;
using ImageIndex           = std::uint32_t;
using Allocator            = VmaAllocator;
using Allocation           = VmaAllocation;
using AllocationCreateInfo = VmaAllocationCreateInfo;

} // namespace rf3d::gfx::vk
