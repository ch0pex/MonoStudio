#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk {

class LogicalDevice {
public:
  using queue_type        = raii::Queue;
  using native_type       = raii::Device;
  using queue_config_type = core::DeviceQueueCreateInfo;

  explicit LogicalDevice(native_type&& device) : handle(std::move(device)) { }

  [[nodiscard]] queue_type create_graphics_queue(queue_config_type const& queue_config) const {
    return {handle, queue_config.queueFamilyIndex, 0};
  }

  [[nodiscard]] queue_type create_present_queue(queue_config_type const& queue_config) const {
    return {handle, queue_config.queueFamilyIndex, queue_config.queueCount > 1U ? 1U : 0U};
  }

  void wait_idle() const { handle.waitIdle(); }

  void wait_for_fences(std::span<core::Fence const> fences, bool wait_all, std::chrono::milliseconds timeout) const {
    handle.waitForFences(
        fences, //
        wait_all ? core::True : core::False, //
        static_cast<std::uint64_t>(timeout.count())
    ) >> check::error;
  }

  void wait_fence(core::Fence const& fence, bool wait_all, std::chrono::milliseconds timeout) const {
    handle.waitForFences(
        fence, //
        wait_all ? core::True : core::False, //
        static_cast<std::uint64_t>(timeout.count())
    ) >> check::error;
  }

  void reset_fences(std::span<core::Fence const> fences) const { handle.resetFences(fences); }

  void reset_fence(core::Fence const& fence) const { handle.resetFences(fence); }

  native_type const& operator*() const noexcept { return handle; }

private:
  native_type handle;
};

} // namespace rf3d::gfx::vk
