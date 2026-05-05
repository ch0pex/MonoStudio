#pragma once

#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

#include <mono/containers/span.hpp>

namespace rf3d::vk::detail {

struct SubmitInfo {
  using native_type = core::SubmitInfo;

  mono::span<core::Semaphore const> wait_semaphores {};
  mono::span<core::PipelineStageFlags const> wait_dst_stage_mask {};
  mono::span<core::CommandBuffer const> command_buffers {};
  mono::span<core::Semaphore const> signal_semaphores {};
};


inline core::SubmitInfo to_native(SubmitInfo const& submit_info) {
  core::SubmitInfo const info {
    .waitSemaphoreCount   = static_cast<std::uint32_t>(submit_info.wait_semaphores.size()),
    .pWaitSemaphores      = submit_info.wait_semaphores.data(),
    .pWaitDstStageMask    = submit_info.wait_dst_stage_mask.data(),
    .commandBufferCount   = static_cast<std::uint32_t>(submit_info.command_buffers.size()),
    .pCommandBuffers      = submit_info.command_buffers.data(),
    .signalSemaphoreCount = static_cast<std::uint32_t>(submit_info.signal_semaphores.size()),
    .pSignalSemaphores    = submit_info.signal_semaphores.data(),
  };

  return info;
}

} // namespace rf3d::vk::detail
