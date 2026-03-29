#pragma once

#include "mono/containers/span.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk::gpu {

struct SubmitInfo {
  using native_type = core::SubmitInfo;

  mono::span<core::Semaphore const> wait_semaphores {}; // NOLINT
  mono::span<core::PipelineStageFlags const> wait_dst_stage_mask {}; // NOLINT
  mono::span<core::CommandBuffer const> command_buffers {}; // NOLINT
  mono::span<core::Semaphore const> signal_semaphores {}; // NOLINT
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

namespace wait {

struct idle_t { };

struct fence_t { };

inline constexpr idle_t idle {};

inline constexpr fence_t fence {};


} // namespace wait


} // namespace rf3d::gfx::vk::gpu
