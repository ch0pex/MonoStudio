#pragma once

#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"

namespace rf3d::hri::vk::detail {

inline raii::Device create_logical_device(PhysicalDevice const& physical_device) {
  static float constexpr queue_priority = 0.5F;
  std::vector<float> priorities {queue_priority, queue_priority};

  core::DeviceQueueCreateInfo queue_create_info {
    .queueFamilyIndex = physical_device.queue_indices().main_family.value(),
    .queueCount       = 2,
    .pQueuePriorities = priorities.data(),
  };

  core::PhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_features {
    .extendedDynamicState = core::True,
  };

  core::PhysicalDeviceVulkan13Features vulkan13_features {
    .pNext            = &extended_dynamic_state_features,
    .dynamicRendering = core::True,
  };

  auto features  = physical_device.getFeatures2();
  features.pNext = &vulkan13_features;

  // Needs static live time
  static auto const extensions = device_extensions();
  core::DeviceCreateInfo create_info {
    .pNext                   = &features,
    .queueCreateInfoCount    = 1,
    .pQueueCreateInfos       = &queue_create_info,
    .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
  };


  return physical_device.createDevice(create_info);
}

} // namespace rf3d::hri::vk::detail
