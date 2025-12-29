#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"

//
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

inline VkDevice create_logical_device(VkPhysicalDevice const physical_device, QueueFamilyIndices const& indices) {
  VkDevice device {nullptr};

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

  // if (enableValidationLayers) {
  //   createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
  //   createInfo.ppEnabledLayerNames = validationLayers.data();
  // }
  // else {
  //   createInfo.enabledLayerCount = 0;
  // }

  vkCreateDevice(physical_device, &create_info, nullptr, &device) >> check::error;

  return device;
}

} // namespace rf3d::hri::vk::detail
