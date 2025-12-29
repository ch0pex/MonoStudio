#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

//
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

inline VkDevice create_logical_device(VkPhysicalDevice const physical_device, QueueFamilyIndices const& indices) {
  VkDevice device {nullptr};
  auto const validation_layers = get_validation_layers();

  VkDeviceQueueCreateInfo queueCreateInfo {};
  queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphics_family.value();
  queueCreateInfo.queueCount       = 1;

  // Not neeeded for now
  VkPhysicalDeviceFeatures device_features {};

  VkDeviceCreateInfo create_info {};
  create_info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos    = &queueCreateInfo;
  create_info.queueCreateInfoCount = 1;
  create_info.pEnabledFeatures     = &device_features;

  create_info.enabledExtensionCount = 0;

  create_info.enabledLayerCount = validation_layers.size();
  if constexpr (enable_validation_layers) {
    create_info.ppEnabledLayerNames = not validation_layers.empty() ? validation_layers.data() : nullptr;
  }

  vkCreateDevice(physical_device, &create_info, nullptr, &device) >> check::error;

  return device;
}

} // namespace rf3d::hri::vk::detail
