#pragma once

#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"

#include <cstdint>

namespace rf3d::gfx::vk {

class CommandPool {
public:
  using config_type = core::CommandPoolCreateInfo;
  using buffer_type = raii::CommandBuffer;
  using native_type = raii::CommandPool;
  using device_type = LogicalDevice;

  explicit CommandPool(device_type const& device, config_type const& config) : handle(*device, config) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = handle,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = 3,
    };

    buffers = vk::raii::CommandBuffers(*device, alloc_info);
  }

  void transition_image_layout(Image::barrier_type const& barrier) { }

  buffer_type& buffer(std::size_t index) { return buffers.at(index); }

private:
  native_type handle;
  std::vector<buffer_type> buffers;
};


} // namespace rf3d::gfx::vk
