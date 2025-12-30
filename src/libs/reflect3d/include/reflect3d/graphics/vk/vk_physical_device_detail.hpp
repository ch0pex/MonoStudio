#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

//
#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::hri::vk::detail {


inline std::vector<std::string> device_extensions() {
  return {
    core::KHRSwapchainExtensionName,
    core::KHRSpirv14ExtensionName,
    core::KHRSynchronization2ExtensionName,
    core::KHRCreateRenderpass2ExtensionName,
  };
}

inline std::uint64_t rate_device(raii::PhysicalDevice const& device) {
  auto device_properties = device.getProperties();
  auto device_features   = device.getFeatures();

  std::uint64_t score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == core::PhysicalDeviceType::eDiscreteGpu) {
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

inline raii::PhysicalDevice pick_best_physical_device(raii::Instance const& instance) {
  std::multimap<std::uint64_t, raii::PhysicalDevice, std::greater<>> candidates;

  for (auto const& device: raii::PhysicalDevices {instance}) {
    candidates.insert({rate_device(device), device});
  }

  if (candidates.empty() or candidates.begin()->first == 0) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  return candidates.begin()->second;
}

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
};

inline QueueFamilyIndices find_queue_families(raii::PhysicalDevice const& physicalDevice) {
  QueueFamilyIndices indices;
  auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

  // get the first index into queueFamilyProperties which supports graphics
  auto const graphics_queue_family =
      std::ranges::find_if(queueFamilyProperties, [](core::QueueFamilyProperties const& qfp) {
        return static_cast<std::uint32_t>(qfp.queueFlags & core::QueueFlagBits::eGraphics) != 0U;
      });

  if (graphics_queue_family != queueFamilyProperties.end()) {
    indices.graphics_family =
        static_cast<std::uint32_t>(std::distance(queueFamilyProperties.begin(), graphics_queue_family));
  }

  return indices;
}

} // namespace rf3d::hri::vk::detail
