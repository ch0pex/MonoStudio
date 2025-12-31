#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_physical_device.hpp"

//
#include <mono/logging/logger.hpp>

//
#include <cstdint>
#include <map>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::hri::vk::detail {

inline std::vector<char const*> device_extensions() {
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

inline PhysicalDevice pick_best_physical_device(raii::Instance const& instance) {
  std::multimap<std::uint64_t, raii::PhysicalDevice, std::greater<>> candidates;

  for (auto const& device: raii::PhysicalDevices {instance}) {
    LOG_INFO("Found GPU: {}", device.getProperties().deviceName.data());
    candidates.insert({rate_device(device), device});
  }

  if (candidates.empty() or candidates.begin()->first == 0) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  return PhysicalDevice {candidates.begin()->second};
}

} // namespace rf3d::hri::vk::detail
