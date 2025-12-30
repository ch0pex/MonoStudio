#pragma once

#include "reflect3d/graphics/vk/vk_logical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"

namespace rf3d::hri::vk {

class Instance;

class Gpu {
public:
  using physical_type       = raii::PhysicalDevice;
  using logical_type        = raii::Device;
  using graphics_queue_type = raii::Queue;

  Gpu(physical_type&& physical, logical_type&& logical) :
    physical_handle(std::move(physical)), //
    logical_handle(std::move(logical)) //
  { }

private:
  physical_type physical_handle;
  logical_type logical_handle;
  // graphics_queue_type graphics_queue;
};

struct BestGpuCriteria {
} constexpr best_gpu_criteria;

struct CompatibleGpuCriteria {
} constexpr compatible_gpu_criteria;

} // namespace rf3d::hri::vk
