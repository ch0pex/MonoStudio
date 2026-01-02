#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk {

class LogicalDevice : raii::Device {
public:
  using queue_type        = raii::Queue;
  using native_type       = raii::Device;
  using queue_config_type = core::DeviceQueueCreateInfo;

  explicit LogicalDevice(native_type&& device) : raii::Device(std::move(device)) { }

  [[nodiscard]] queue_type create_graphics_queue(queue_config_type const& queue_config) const {
    return {*this, queue_config.queueFamilyIndex, 0};
  }

  [[nodiscard]] queue_type create_present_queue(queue_config_type const& queue_config) const {
    return {*this, queue_config.queueFamilyIndex, queue_config.queueCount > 1U ? 1U : 0U};
  }
};

} // namespace rf3d::gfx::vk
