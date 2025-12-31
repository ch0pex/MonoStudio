#pragma once

#include "reflect3d/graphics/vk/vk_graphics_families.hpp"
#include "reflect3d/graphics/vk/vk_logical_device_detail.hpp"

#include <mono/misc/passkey.hpp>

namespace rf3d::hri::vk {

class Instance;

struct GpuDevices {
  using logical_type  = raii::Device;
  using physical_type = PhysicalDevice;

  explicit GpuDevices(physical_type const& physical) :
    physical(physical), //
    logical(detail::create_logical_device(physical)) //
  { }

  physical_type physical;
  logical_type logical;
};

struct GpuQueues {
  using queue_type  = raii::Queue;
  using device_type = raii::Device;

  GpuQueues(raii::Device& logical, QueueFamilyIndices const& indices) :
    graphics(logical, indices.main_family.value(), 0), //
    present(logical, indices.main_family.value(), 1) //
  { }

  queue_type graphics;
  queue_type present;
};

class Gpu {
public:
  using queues_type  = GpuQueues;
  using devices_type = GpuDevices;

  Gpu(devices_type&& gpu_devices, mono::PassKey<Instance> key [[maybe_unused]]) :
    devices(std::move(gpu_devices)), //
    queues(devices.logical, devices.physical.queue_indices()) //
  { }

private:
  devices_type devices;
  queues_type queues;
};

struct BestGpuCriteria {
} constexpr best_gpu_criteria;

struct CompatibleGpuCriteria {
} constexpr compatible_gpu_criteria;

} // namespace rf3d::hri::vk
