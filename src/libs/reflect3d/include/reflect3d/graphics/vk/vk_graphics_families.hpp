#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

//
#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::hri::vk {

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
};

namespace detail {

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

} // namespace detail

} // namespace rf3d::hri::vk
