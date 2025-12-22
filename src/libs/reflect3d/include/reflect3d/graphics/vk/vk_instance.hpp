#pragma once

#include "reflect3d/graphics/instance_config.hpp"
#include "reflect3d/graphics/vk/vk_exception.hpp"
#include "reflect3d/graphics/vk/vk_instance_detail.hpp"

#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {

class Instance {
public:
  explicit Instance(InstanceConfig const& config) {
    VkInstanceCreateInfo createInfo = detail::create_instance_info(config);

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      throw Exception("Failed to create vulkan instance!");
    }
  }

  ~Instance() { vkDestroyInstance(instance, nullptr); }

  Instance(Instance const&) = default;

  Instance(Instance&&) = delete;

  Instance& operator=(Instance const&) = default;

  Instance& operator=(Instance&&) = delete;

private:
  VkInstance instance {};
};

} // namespace rf3d::hri::vk
