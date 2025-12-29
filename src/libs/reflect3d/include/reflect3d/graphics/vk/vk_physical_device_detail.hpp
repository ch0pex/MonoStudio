#pragma once

//
#include <vulkan/vulkan_core.h>

//
#include <cstdint>
#include <map>
#include <ranges>
#include <vector>

namespace rf3d::hri::vk::detail {

inline std::vector<VkPhysicalDevice> enumerate_physical_devices(VkInstance const instance) {
  std::uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

  return devices;
}

inline std::uint64_t rate_device(VkPhysicalDevice const device) {
  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;

  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);

  std::uint64_t score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += device_properties.limits.maxImageDimension2D;
  score += device_properties.limits.maxImageDimension3D;
  score += device_properties.limits.maxImageDimensionCube;
  score += device_properties.limits.maxComputeSharedMemorySize;

  // Application can't function without geometry shaders
  if (device_features.geometryShader == VK_FALSE) {
    return 0;
  }

  return score;
}

inline VkPhysicalDevice pick_physical_device(VkInstance const instance) {
  std::multimap<std::uint64_t, VkPhysicalDevice, std::greater<>> candidates;

  for (auto const& device: enumerate_physical_devices(instance)) {
    std::uint64_t const score = rate_device(device);
    candidates.insert({score, device});
  }

  if (candidates.empty() or candidates.begin()->first == 0) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  return candidates.begin()->second;
}

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
};

QueueFamilyIndices find_queue_families(VkPhysicalDevice const device) {
  QueueFamilyIndices indices;

  std::uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  for (int i = 0; auto const& queue_family: queue_families) {
    if ((queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0U) {
      indices.graphics_family = i;
    }
    ++i;
  }

  return indices;
}

} // namespace rf3d::hri::vk::detail
