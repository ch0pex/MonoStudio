#pragma once

#include "reflect3d/graphics/instance_config.hpp"
#include "reflect3d/graphics/vk/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/vk_instance_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {

struct BaseInstance {
  // --- Type Traits ---
  using handle_type = VkInstance;

  // --- Constructors ---
  explicit BaseInstance(InstanceConfig const& config = {}) : handle(detail::create_instance(config)) { }

  ~BaseInstance() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyInstance(handle, nullptr);
      handle = VK_NULL_HANDLE;
    }
  }

  BaseInstance(BaseInstance const&) = delete;

  BaseInstance& operator=(BaseInstance const&) = delete;

  BaseInstance(BaseInstance&& other) noexcept : handle(std::exchange(other.handle, VK_NULL_HANDLE)) { }

  BaseInstance& operator=(BaseInstance&& other) noexcept {
    if (this != &other) {
      if (handle != VK_NULL_HANDLE) {
        vkDestroyInstance(handle, nullptr);
      }
      handle = std::exchange(other.handle, VK_NULL_HANDLE);
    }
    return *this;
  }

  handle_type handle {VK_NULL_HANDLE};
};

} // namespace rf3d::hri::vk
