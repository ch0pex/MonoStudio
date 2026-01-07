#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"

#include <cstdint>

namespace rf3d::gfx::vk {

class CommandPool {
public:
  using config_type = core::CommandPoolCreateInfo;
  using buffer_type = raii::CommandBuffer;
  using native_type = raii::CommandPool;
  using device_type = LogicalDevice;
  using fence_type  = raii::Fence;

  explicit CommandPool(device_type const& device, config_type const& config) : handle(*device, config) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = handle,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = defaults::max_frames_in_flight
    };

    buffers = vk::raii::CommandBuffers(*device, alloc_info);
    for (std::size_t i = 0; i < defaults::max_frames_in_flight; ++i) {
      draw_fences.emplace_back(*device, core::FenceCreateInfo {.flags = core::FenceCreateFlagBits::eSignaled});
    }
  }

  [[nodiscard]] buffer_type const& next_command_buffer() {
    current_frame = (current_frame + 1U) % buffers.size();
    return buffers.at(current_frame);
  }

  [[nodiscard]] FrameIndex current_frame_index() const noexcept { return current_frame; }

  [[nodiscard]] fence_type const& fence() const { return draw_fences.at(current_frame); }

private:
  native_type handle;
  std::vector<buffer_type> buffers;
  std::vector<raii::Fence> draw_fences;
  std::uint32_t current_frame {0};
};


} // namespace rf3d::gfx::vk
