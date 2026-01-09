#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"

#include <mono/containers/stable_vector.hpp>

#include <cstdint>
#include <ranges>

namespace rf3d::gfx::vk {

namespace details { } // namespace details
//

class CommandPool {
public:
  using config_type = core::CommandPoolCreateInfo;
  using buffer_type = CommandBuffer;
  using device_type = LogicalDevice;
  using native_type = raii::CommandPool;
  using fence_type  = raii::Fence;

  explicit CommandPool(device_type const& device, config_type const& config) : handle(*device, config) {
    allocate_command_buffers(device, defaults::max_frames_in_flight);
  }

  [[nodiscard]] buffer_type const& next_command_buffer() {
    current_buffer = (current_buffer + 1U) % buffers.size();
    return buffers.at(current_buffer);
  }

  void allocate_command_buffers(device_type const& device, std::uint32_t const count) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = handle,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = count,
    };

    for (auto& buffer: raii::CommandBuffers(*device, alloc_info)) {
      buffers.emplace_back(CommandBuffer {std::move(buffer)});
    }

    for (std::size_t i = 0; i < defaults::max_frames_in_flight; ++i) {
      draw_fences.emplace_back(*device, core::FenceCreateInfo {.flags = core::FenceCreateFlagBits::eSignaled});
    }
  }

  void deallocate_command_buffers(std::uint32_t const count) {
    std::uint32_t const dealloc_count = std::min<std::uint32_t>(count, buffers.size());
    if (dealloc_count > 0) {
      buffers.erase(buffers.end() - dealloc_count, buffers.end());
    }
  }

  [[nodiscard]] fence_type const& fence() const { return draw_fences.at(current_buffer); }

  [[nodiscard]] FrameIndex current_frame_index() const noexcept { return current_buffer; }

private:
  native_type handle;
  mono::stable_vector<buffer_type> buffers;
  mono::stable_vector<raii::Fence> draw_fences;
  std::uint32_t current_buffer {0};
};


} // namespace rf3d::gfx::vk
