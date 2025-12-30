#pragma once

#include "reflect3d/graphics/vk/vk_logical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"

namespace rf3d::hri::vk {

class Instance;

class Gpu {
public:
  using physical_type       = PhysicalDevice;
  using logical_type        = raii::Device;
  using graphics_queue_type = raii::Queue;

  Gpu(physical_type&& physical, logical_type&& logical) :
    physical_device(std::move(physical)), //
    logical_device(std::move(logical)), //
    graphics_queue(logical_device, physical_device.queue_indices().graphics_family.value(), 0U) //
  { }


private:
  physical_type physical_device;
  logical_type logical_device;
  graphics_queue_type graphics_queue;
};

struct BestGpuCriteria {
} constexpr best_gpu_criteria;

struct CompatibleGpuCriteria {
} constexpr compatible_gpu_criteria;

} // namespace rf3d::hri::vk
