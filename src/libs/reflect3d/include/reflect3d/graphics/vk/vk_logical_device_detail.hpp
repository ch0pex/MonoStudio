#pragma once

#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

//
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

inline raii::Device create_logical_device(
    raii::Context const& context, raii::PhysicalDevice const& physical_device, QueueFamilyIndices const& indices
) {
  auto const validation_layers = get_validation_layers(context);

  core::DeviceQueueCreateInfo queueCreateInfo {};
  queueCreateInfo.queueFamilyIndex = indices.graphics_family.value();
  queueCreateInfo.queueCount       = 1;

  // Not neeeded for now
  core::PhysicalDeviceFeatures device_features {};

  core::DeviceCreateInfo create_info {};
  create_info.pQueueCreateInfos    = &queueCreateInfo;
  create_info.queueCreateInfoCount = 1;
  create_info.pEnabledFeatures     = &device_features;

  create_info.enabledExtensionCount = 0;

  create_info.enabledLayerCount = validation_layers.size();
  if constexpr (enable_validation_layers) {
    create_info.ppEnabledLayerNames = not validation_layers.empty() ? validation_layers.data() : nullptr;
  }

  return physical_device.createDevice(create_info);
}

} // namespace rf3d::hri::vk::detail
