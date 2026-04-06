#pragma once

#include "reflect3d/graphics/vk/detail/gpu/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

#include <mono/containers/stable_vector.hpp>

#include <cstdint>

namespace rf3d::vk::detail {

class CommandPools {
public:
  using pool_type            = raii::CommandPool;
  using buffer_type          = raii::CommandBuffer;
  using device_type          = LogicalDevice;
  using physical_device_type = PhysicalDevice;
  using buffers_type         = raii::CommandBuffers;

  explicit CommandPools(device_type& device, physical_device_type const& physical) :
    primary_pool(
        *device,
        core::CommandPoolCreateInfo {
          .flags            = core::CommandPoolCreateFlagBits::eResetCommandBuffer,
          .queueFamilyIndex = physical.queue_creation_info().family_info<QueueFamilyType::Main>().queueFamilyIndex,
        }
    ),
    unique_use_pool(
        *device,
        core::CommandPoolCreateInfo {
          .flags            = core::CommandPoolCreateFlagBits::eTransient,
          .queueFamilyIndex = physical.queue_creation_info().family_info<QueueFamilyType::Main>().queueFamilyIndex,
        }
    ),
    gpu_device(std::addressof(device)) { }

  buffers_type allocate_transient_buffers(std::uint32_t const count) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = unique_use_pool,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = count,
    };

    return raii::CommandBuffers {**gpu_device, alloc_info}; //
  }

  buffers_type allocate_primary_buffers(std::uint32_t const count) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = primary_pool,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = count,
    };
    return raii::CommandBuffers {**gpu_device, alloc_info}; //
  }

private:
  pool_type primary_pool;
  pool_type unique_use_pool;
  device_type* gpu_device;
};

} // namespace rf3d::vk::detail
