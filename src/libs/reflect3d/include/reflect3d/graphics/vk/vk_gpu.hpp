#pragma once


#include <vulkan/vulkan_core.h>
#include "reflect3d/graphics/vk/vk_logical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"

namespace rf3d::hri::vk {

class Gpu {
public:
  using physical_type       = VkPhysicalDevice;
  using logical_type        = VkDevice;
  using graphics_queue_type = VkQueue;

  explicit Gpu(VkInstance instance) : physical_handle(detail::pick_physical_device(instance)) {
    auto indices = detail::find_queue_families(physical_handle);

    logical_handle = detail::create_logical_device(physical_handle, indices);

    vkGetDeviceQueue(logical_handle, indices.graphics_family.value(), 0, &graphics_queue);
  }

  ~Gpu() {
    if (logical_handle != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(logical_handle);
      vkDestroyDevice(logical_handle, nullptr);
      logical_handle = VK_NULL_HANDLE;
    }
  }

  Gpu(Gpu const&)            = delete;
  Gpu& operator=(Gpu const&) = delete;

  Gpu(Gpu&& other) noexcept :
    physical_handle(std::exchange(other.physical_handle, nullptr)),
    logical_handle(std::exchange(other.logical_handle, VK_NULL_HANDLE)),
    graphics_queue(std::exchange(other.graphics_queue, VK_NULL_HANDLE)) { }

  Gpu& operator=(Gpu&& other) noexcept {
    if (this != std::addressof(other)) {
      // 1. Limpiar recursos actuales
      if (logical_handle != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(logical_handle);
        vkDestroyDevice(logical_handle, nullptr);
      }

      physical_handle = std::exchange(other.physical_handle, nullptr);
      logical_handle  = std::exchange(other.logical_handle, VK_NULL_HANDLE);
      graphics_queue  = std::exchange(other.graphics_queue, VK_NULL_HANDLE);
    }
    return *this;
  }

  [[nodiscard]] physical_type get_physical() const { return physical_handle; }

  [[nodiscard]] logical_type get_logical() const { return logical_handle; }

  [[nodiscard]] graphics_queue_type get_queue() const { return graphics_queue; }

private:
  physical_type physical_handle {nullptr};
  logical_type logical_handle {nullptr};
  VkQueue graphics_queue {nullptr};
};

} // namespace rf3d::hri::vk
