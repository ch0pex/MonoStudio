/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_native_types.hpp
 * @date 26/05/2026
 * @brief Vulkan native type aliases and namespace definitions
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_raii.hpp>

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

namespace core = ::vk;
namespace raii = ::vk::raii;

using Allocator            = VmaAllocator;
using Allocation           = VmaAllocation;
using AllocationInfo       = VmaAllocationInfo;
using AllocationCreateInfo = VmaAllocationCreateInfo;

} // namespace rf3d::vk::detail
