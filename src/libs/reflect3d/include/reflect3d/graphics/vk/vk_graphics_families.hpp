#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

//
#include <algorithm>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::gfx::vk {

struct QueueFamilyIndices {
  std::optional<std::uint32_t> main_family;

  [[nodiscard]] bool is_complete() const noexcept { return main_family.has_value(); }
};

namespace detail {

inline QueueFamilyIndices find_queue_families(raii::PhysicalDevice const& physical_device) {
  QueueFamilyIndices indices;
  auto const queueFamilyProperties = physical_device.getQueueFamilyProperties();

  // get the first index into queueFamilyProperties which supports graphics
  auto const graphics_queue_family =
      std::ranges::find_if(queueFamilyProperties, [](core::QueueFamilyProperties const& qfp) {
        return static_cast<std::uint32_t>(qfp.queueFlags & core::QueueFlagBits::eGraphics) != 0U and
               qfp.queueCount >= 2;
      });

  if (graphics_queue_family != queueFamilyProperties.end()) {
    indices.main_family = static_cast<std::uint32_t>( //
        std::distance(queueFamilyProperties.begin(), graphics_queue_family)
    );
  }
  else {
    throw std::runtime_error("Failed to find graphics queue family for graphics and present operations");
  }

  return indices;
}

} // namespace detail

} // namespace rf3d::gfx::vk
