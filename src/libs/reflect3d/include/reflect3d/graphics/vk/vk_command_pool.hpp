#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"

#include <cstdint>
#include <vulkan/vulkan_raii.hpp>

namespace rf3d::gfx::vk {

class CommandPool {
public:
  using config_type = core::CommandPoolCreateInfo;
  using buffer_type = CommandBuffer;
  using device_type = LogicalDevice;
  using native_type = raii::CommandPool;
  using fence_type  = raii::Fence;

  explicit CommandPool(device_type const& device, config_type const& config) : handle(*device, config) {
    core::CommandBufferAllocateInfo alloc_info {
      .commandPool        = handle,
      .level              = core::CommandBufferLevel::ePrimary,
      .commandBufferCount = defaults::max_frames_in_flight
    };

    auto native_buffers = vk::raii::CommandBuffers(*device, alloc_info);
    // | std::views::transform([](raii::CommandBuffer& buffer) { return CommandBuffer {std::move(buffer)}; }) //
    // | std::views::to<std::vector<buffer_type>>(); //
    for (auto& native_buffer: native_buffers) {
      buffers.emplace_back(std::move(native_buffer));
    }

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
