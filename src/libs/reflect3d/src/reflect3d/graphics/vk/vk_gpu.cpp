#include "reflect3d/graphics/vk/vk_gpu.hpp"

#include "reflect3d/graphics/vk/gpu/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/gpu/vk_command_pool.hpp"
#include "reflect3d/graphics/vk/gpu/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/gpu/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/gpu/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"

//
#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>


//
#include <assets_path.hpp>

//
#include <cassert>

namespace rf3d::gfx::vk::gpu {

namespace {


struct Gpu {
  /**********************
   *    Constructors    *
   **********************/
  Gpu() = default;

  Gpu(Gpu const&) = delete;

  Gpu(Gpu&&) = delete;

  Gpu& operator=(Gpu const&) = delete;

  Gpu& operator=(Gpu&&) = delete;

  ~Gpu() { vmaDestroyAllocator(memory_allocator); }

  /**************************
   *    Member variables    *
   **************************/

  PhysicalDevice physical {pick_best_physical_device()};
  LogicalDevice logical {physical.create_logical_device()};
  Queues<GraphicQueue, PresentQueue> queues {logical, physical.queue_creation_info()};
  VmaAllocator memory_allocator {instance::create_allocator(*physical, *logical)};
  CommandPool command_pool {
    logical, CommandPool::config_type {
               .flags            = core::CommandPoolCreateFlagBits::eResetCommandBuffer,
               .queueFamilyIndex = physical.queue_creation_info().family_info<QueueFamilyType::Main>().queueFamilyIndex,
             }
  };
};

Gpu& get_gpu() {
  static Gpu instance {};
  return instance;
}

} // namespace

// ----------------------------------------------
// --- Gpu frame commands and synchronization ---
// ----------------------------------------------

void wait_idle() { //
  get_gpu().logical.wait_idle();
}

FrameIndex next_frame() {
  auto& gpu              = get_gpu();
  auto const frame_index = gpu.command_pool.next_frame();

  auto const& fence = gpu.command_pool.fence();
  gpu.logical.wait_fence(*fence, true, std::chrono::milliseconds {defaults::wait_timeout});

  return frame_index;
}

core::CommandBuffer record_commands(
    CommandBuffer::begin_info const& begin_info, //
    std::function<void(CommandBuffer const&)> const& command_recording_func //
) {
  auto& gpu          = get_gpu();
  auto const& buffer = gpu.command_pool.command_buffer();

  gpu.logical.reset_fence(*gpu.command_pool.fence());
  buffer.record(begin_info, command_recording_func);

  return **buffer;
}

void submit_work(
    std::span<core::Semaphore const> wait_semaphores, //
    std::span<core::CommandBuffer const> command_buffers, //
    std::span<core::Semaphore const> signal_semaphores //
) {
  static constexpr core::PipelineStageFlags mask = core::PipelineStageFlagBits::eColorAttachmentOutput;
  core::SubmitInfo const info {
    .waitSemaphoreCount   = static_cast<std::uint32_t>(wait_semaphores.size()),
    .pWaitSemaphores      = wait_semaphores.data(),
    .pWaitDstStageMask    = std::addressof(mask),
    .commandBufferCount   = static_cast<std::uint32_t>(command_buffers.size()),
    .pCommandBuffers      = command_buffers.data(),
    .signalSemaphoreCount = static_cast<std::uint32_t>(signal_semaphores.size()),
    .pSignalSemaphores    = signal_semaphores.data(),
  };

  get_gpu().queues.graphics().submit(info, get_gpu().command_pool.fence());
}

mono::err::expected<void> present(core::PresentInfoKHR const& present_info) {
  auto const result = get_gpu().queues.present().presentKHR(present_info);
  if (result == core::Result::eSuboptimalKHR or result == core::Result::eErrorOutOfDateKHR) {
    return mono::err::unexpected("Swapchain is out of date or suboptimal");
  }
  assert(result == core::Result::eSuccess);
  return mono::err::expected<void> {};
}

// --------------------------------------
// --- Gpu resource factory functions ---
// --------------------------------------

raii::ShaderModule make_shader_module(core::ShaderModuleCreateInfo const& module_info) {
  return {*get_gpu().logical, module_info};
}

raii::ImageView make_image_view(core::ImageViewCreateInfo const& view_info) { //
  return {*get_gpu().logical, view_info};
}

raii::SwapchainKHR make_swapchain(core::SwapchainCreateInfoKHR const& swapchain_info) {
  return {*get_gpu().logical, swapchain_info};
}

raii::Semaphore make_semaphore(core::SemaphoreCreateInfo const& semaphore_info) {
  return {*get_gpu().logical, semaphore_info};
}

raii::Pipeline make_graphics_pipeline(core::GraphicsPipelineCreateInfo const& pipeline_info) {
  return {*get_gpu().logical, nullptr, pipeline_info};
}

raii::PipelineLayout make_pipeline_layout(core::PipelineLayoutCreateInfo const& layout_info) {
  return {*get_gpu().logical, layout_info};
}

BufferAllocation allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info) {
  VkBuffer buffer          = nullptr;
  VmaAllocation allocation = nullptr;
  VmaAllocationInfo allocation_info;

  VkBufferCreateInfo const vk_buffer_info = buf_info;
  vmaCreateBuffer(
      get_gpu().memory_allocator,
      &vk_buffer_info, //
      &alloc_info, //
      &buffer, //
      &allocation, //
      &allocation_info //
  );

  return {
    .buffer_handle     = buffer,
    .allocation_handle = allocation,
    .allocation_info   = allocation_info,
  };
}

void free_buffer(BufferAllocation const& buffer_allocation) {
  vmaDestroyBuffer(get_gpu().memory_allocator, buffer_allocation.buffer_handle, buffer_allocation.allocation_handle);
}

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface) { //
  return get_gpu().physical.get_surface_info(surface);
}

} // namespace rf3d::gfx::vk::gpu
