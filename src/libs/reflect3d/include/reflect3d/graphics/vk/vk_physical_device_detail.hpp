#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_physical_device.hpp"

//
#include <mono/logging/logger.hpp>

//

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#endif


namespace rf3d::gfx::vk::detail {

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
  if (device_features.geometryShader == core::False) {
    return 0;
  }

  return score;
}

inline PhysicalDevice pick_best_physical_device(raii::Instance const& instance) {
  std::multimap<std::uint64_t, raii::PhysicalDevice, std::greater<>> candidates;

  for (auto const& device: raii::PhysicalDevices {instance}) {
    LOG_INFO("Found GPU: {}", std::string_view {device.getProperties().deviceName});
    candidates.insert({rate_device(device), device});
  }

  if (candidates.empty() or candidates.begin()->first == 0) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  return PhysicalDevice {candidates.begin()->second};
}

} // namespace rf3d::gfx::vk::detail
