#pragma once

#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/vk_command_pool.hpp"
#include "reflect3d/graphics/vk/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_pso_builder.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <assets_path.hpp>
#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace rf3d::gfx::vk {

class Instance; // forward declaration

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
    gpu_queues(devices.logical, devices.physical.queue_creation_info()), //
    command_pool(
        devices.logical,
        CommandPool::config_type {
          .flags = core::CommandPoolCreateFlagBits::eResetCommandBuffer, //
          .queueFamilyIndex =
              devices.physical.queue_creation_info().family_info<QueueFamilyType::Main>().queueFamilyIndex, //
        }
    ) { }

  Swapchain create_swapchain(raii::SurfaceKHR&& surface, Resolution const& resolution) {
    auto const swapchain_config = create_swapchain_config(
        surface, //
        devices.physical.get_surface_info(*surface), //
        resolution
    );

    auto swapchain = Swapchain(*devices.logical, std::move(surface), swapchain_config);

    // Provisional pipeline for testing
    auto const assets_path = std::filesystem::path {mono::assets_path};
    auto const shader_path = assets_path / "shaders" / "bin" / "reflect3d-shaders.spv";

    auto shader = devices.logical.create_shader(gfx::vk::load_shader_bytecode(shader_path));

    auto pso = PipelineBuilder({.color_attachment_format = swapchain_config.imageFormat}) //
                   .vertex_stage(shader)
                   .fragment_stage(shader)
                   .build(*devices.logical);


    psos.insert({0, std::move(pso)});
    return swapchain;
  }

  // queues_type const& queues() const noexcept { return gpu_queues; }

  [[nodiscard]] std::tuple<std::uint32_t, CommandPool::buffer_type const&> command_buffer() {
    auto const& buffer = command_pool.next_command_buffer();
    auto const& fence  = command_pool.fence();

    (*devices.logical).waitForFences(*fence, core::True, defaults::wait_timeout) >> check::error;
    (*devices.logical).resetFences(*fence);


    return {command_pool.current_frame_index(), buffer};
  }

  template<Queue QueueType>
  void submit_work(core::SubmitInfo const& info) {
    gpu_queues.template get<QueueType>().submit(info, command_pool.fence());
  }

  void present(core::PresentInfoKHR const& present_info)
    requires(mono::meta::in_pack<PresentQueue, Types...>)
  {
    gpu_queues.present().presentKHR(present_info) >> check::error;
  }

  [[nodiscard]] Pipeline const& pipeline() const { return psos.at(0); }

private:
  devices_type devices;
  queues_type gpu_queues;
  CommandPool command_pool;
  std::unordered_map<std::uint32_t, Pipeline> psos;
};

using HeadlessGraphicsGpu = Gpu<GraphicQueue>;

using ComputeGpu = Gpu<ComputeQueue>;

using GraphicsGpu = Gpu<GraphicQueue, PresentQueue>;


} // namespace rf3d::gfx::vk
