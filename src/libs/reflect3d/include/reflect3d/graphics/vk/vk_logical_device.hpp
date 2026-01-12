#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_shader.hpp"

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

  Shader create_shader(Shader::byte_code_type&& bytecode) const { return Shader(handle, std::move(bytecode)); }

  native_type const& operator*() const noexcept { return handle; }

private:
  native_type handle;
};

} // namespace rf3d::gfx::vk
