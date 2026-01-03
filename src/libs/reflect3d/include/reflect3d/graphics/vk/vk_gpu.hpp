#pragma once

#include "reflect3d/graphics/vk/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>

namespace rf3d::gfx::vk {

class Instance; // forward declaration
//
struct GpuDevices {
  using logical_type  = LogicalDevice;
  using physical_type = PhysicalDevice;

  explicit GpuDevices(physical_type&& physical_device) :
    physical(std::move(physical_device)), //
    logical(physical.create_logical_device()) //
  { }

  physical_type physical;
  logical_type logical;
};


/**
 * @brief Represents a GPU device along with its logical device and associated queues.
 *
 * In reflect3d, a Gpu represents a physical GPU device along with its logical device and associated queues.
 * User can specify which queues they want to use by providing the corresponding queue types as template parameters.
 * If there is not a gpu supporting the requested queues construction will raise an exception.
 *
 * @tparam Types Variadic template parameter representing the types of queues to be used with this GPU.
 */
template<Queue... Types>
  requires(sizeof...(Types) >= 1)
class Gpu {
public:
  using queues_type  = Queues<Types...>;
  using devices_type = GpuDevices;

  Gpu( //
      devices_type::physical_type&& physical_device, //
      mono::PassKey<Instance> key [[maybe_unused]]
  ) :
    devices(std::move(physical_device)), //
    gpu_queues(devices.logical, devices.physical.queue_creation_info()) //
  { }

  queues_type const& queues() const noexcept { return gpu_queues; }

  Swapchain create_swap_chain(raii::SurfaceKHR&& surface, Resolution const& resolution) const {
    auto const swapchain_config = create_swapchain_config(
        surface, //
        devices.physical.get_surface_info(*surface), //
        resolution
    );
    return devices.logical.create_swap_chain(std::move(surface), swapchain_config);
  }

private:
  devices_type devices;
  queues_type gpu_queues;
};

using HeadlessGraphicsGpu = Gpu<GraphicQueue>;

using ComputeGpu = Gpu<ComputeQueue>;

using GraphicsGpu = Gpu<GraphicQueue, PresentQueue>;


} // namespace rf3d::gfx::vk
