#pragma once

#include <chrono>
#include "reflect3d/graphics/vk2/detail/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp"

namespace rf3d::vk::detail {

class LogicalDevice {
public:
  using queue_type        = raii::Queue;
  using native_type       = raii::Device;
  using queue_config_type = core::DeviceQueueCreateInfo;

  explicit LogicalDevice(native_type&& device) : logical_device(std::move(device)) { }

  [[nodiscard]] queue_type create_graphics_queue(queue_config_type const& queue_config) const {
    return {logical_device, queue_config.queueFamilyIndex, 0};
  }

  [[nodiscard]] queue_type create_present_queue(queue_config_type const& queue_config) const {
    return {logical_device, queue_config.queueFamilyIndex, queue_config.queueCount > 1U ? 1U : 0U};
  }

  void wait_idle() const { logical_device.waitIdle(); }

  void wait_for_fences(mono::span<core::Fence const> fences, bool wait_all, std::chrono::nanoseconds timeout) const {
    logical_device.waitForFences(
        fences, //
        wait_all ? core::True : core::False, //
        static_cast<std::uint64_t>(timeout.count())
    ) >> check::error;
  }

  void wait_fence(core::Fence const& fence, bool wait_all, std::chrono::nanoseconds timeout) const {
    logical_device.waitForFences(
        fence, //
        wait_all ? core::True : core::False, //
        static_cast<std::uint64_t>(timeout.count())
    ) >> check::error;
  }

  void reset_fences(mono::span<core::Fence const> fences) const { logical_device.resetFences(fences); }

  void reset_fence(core::Fence const& fence) const { logical_device.resetFences(fence); }

  /**
   * Waits for the semaphore to reach a SPECIFIC value.
   */
  void wait_semaphores(
      mono::span<core::Semaphore const> semaphore, //
      mono::span<std::uint64_t const> wait_values, //
      std::chrono::milliseconds timeout //
  ) const {
    logical_device.waitSemaphores(
        core::SemaphoreWaitInfo {
          .semaphoreCount = static_cast<std::uint32_t>(semaphore.size()),
          .pSemaphores    = semaphore.data(),
          .pValues        = wait_values.data(),
        },
        static_cast<std::uint64_t>(timeout.count())
    ) >> check::error;
  }

  /**
   * Signals the semaphore with a NEW value.
   * Note: The new value MUST be greater than the current value.
   */
  void signal_semaphore(core::Semaphore const& semaphore, std::uint64_t const signalValue) const {
    logical_device.signalSemaphoreKHR(
        core::SemaphoreSignalInfo {
          .semaphore = semaphore,
          .value     = signalValue,
        }
    );
  }

  native_type const& operator*() const noexcept { return logical_device; }

  [[nodiscard]] native_type const& handle() const noexcept { return logical_device; }

private:
  native_type logical_device;
};

} // namespace rf3d::vk::detail
