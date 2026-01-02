#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

//
#include <array>

namespace rf3d::gfx::vk::detail {

constexpr auto support_graphics = [](core::QueueFamilyProperties const& properties) {
  return static_cast<std::uint32_t>(properties.queueFlags & core::QueueFlagBits::eGraphics) != 0U;
};

constexpr auto support_compute = [](core::QueueFamilyProperties const& properties) {
  return static_cast<std::uint32_t>(properties.queueFlags & core::QueueFlagBits::eCompute) != 0U;
};

constexpr auto support_transfer = [](core::QueueFamilyProperties const& properties) {
  return static_cast<std::uint32_t>(properties.queueFlags & core::QueueFlagBits::eTransfer) != 0U;
};


inline core::DeviceQueueCreateInfo find_main_family(raii::PhysicalDevice const& physical_device) {
  static constexpr std::array main_family_priorities {1.0F, 1.0F, 1.0F};
  auto const family_properties = physical_device.getQueueFamilyProperties();

  static constexpr auto graphcis_and_compute = [](core::QueueFamilyProperties const& properties) {
    return support_graphics(properties) && support_compute(properties);
  };

  auto graphics_queue_family = std::ranges::find_if(family_properties, graphcis_and_compute);

  if (graphics_queue_family == family_properties.end()) {
    graphics_queue_family = std::ranges::find_if(family_properties, support_graphics);
  }

  if (graphics_queue_family == family_properties.end()) {
    throw std::runtime_error("Failed to find graphics queue family for graphics and present operations");
  }

  auto const index = static_cast<std::uint32_t>( //
      std::distance(family_properties.begin(), graphics_queue_family)
  );

  auto const queue_count = std::min(3U, graphics_queue_family->queueCount);

  if (queue_count < 1U) {
    throw std::runtime_error("Selected graphics queue family has no available queues");
  }

  return {
    .queueFamilyIndex = index,
    .queueCount       = queue_count,
    .pQueuePriorities = main_family_priorities.data(),
  };
}

} // namespace rf3d::gfx::vk::detail
