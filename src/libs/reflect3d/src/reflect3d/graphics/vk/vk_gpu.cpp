#include "reflect3d/graphics/vk/vk_gpu.hpp"

#include "reflect3d/graphics/vk/gpu/vk_command_pool.hpp"
#include "reflect3d/graphics/vk/gpu/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/gpu/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/gpu/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/memory/vk_memory_allocator.hpp"
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"

//
#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>
#include <mono/misc/as_span.hpp>

//
#include <assets_path.hpp>

//
#include <cassert>
#include <vulkan/vulkan_core.h>

namespace rf3d::gfx::vk::gpu {

namespace {

struct Gpu {
  Gpu(PhysicalDevice&& physical_device) :
    physical(std::move(physical_device)), //
    logical(physical.create_logical_device()), //
    memory_allocator(instance::create_allocator(*physical, *logical)), //
    queues(logical, physical.queue_creation_info()), //
    command_pool {
      logical,
      CommandPool::config_type {
        .flags            = core::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = physical.queue_creation_info().family_info<QueueFamilyType::Main>().queueFamilyIndex,
      }
    } { }

  PhysicalDevice physical;
  LogicalDevice logical;
  MemoryAllocator memory_allocator;
  Queues<GraphicQueue, PresentQueue> queues;
  CommandPool command_pool;
};

Gpu& get_gpu() {
  static Gpu instance {pick_best_physical_device()};
  return instance;
}

} // namespace

// ----------------------------------------------
// --- Gpu frame commands and synchronization ---
// ----------------------------------------------

void wait_idle() { //
  LOG_INFO("Waiting for GPU to be idle...");
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

void submit_work(SubmitInfo const& submit_info, [[maybe_unused]] wait::fence_t const wait) {
  get_gpu().queues.graphics().submit(to_native(submit_info), get_gpu().command_pool.fence());
}

void submit_work(SubmitInfo const& submit_info, [[maybe_unused]] wait::idle_t const wait) {
  get_gpu().queues.graphics().submit(to_native(submit_info));
  wait_idle();
}

mono::expected<void> present(core::PresentInfoKHR const& present_info) {
  auto const result = get_gpu().queues.present().presentKHR(present_info);
  if (result == core::Result::eSuboptimalKHR or result == core::Result::eErrorOutOfDateKHR) {
    return mono::unexpected("Swapchain is out of date or suboptimal");
  }
  assert(result == core::Result::eSuccess);
  return mono::expected<void> {};
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

// -------------------------------------
// --- Gpu memory resource functions ---
// -------------------------------------

BufferAllocation allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info) {
  VkBuffer buffer          = nullptr;
  VmaAllocation allocation = nullptr;
  VmaAllocationInfo allocation_info;

  VkBufferCreateInfo const vk_buffer_info = buf_info;
  vmaCreateBuffer(
      get_gpu().memory_allocator.handle, //
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
  vmaDestroyBuffer(
      get_gpu().memory_allocator.handle, buffer_allocation.buffer_handle, buffer_allocation.allocation_handle
  );
}

void upload_buffer(core::Buffer dst_buffer, core::Buffer staging_buffer, core::BufferCopy const& copy_region) {
  core::CommandBufferAllocateInfo const alloc_info {
    .commandPool        = *get_gpu().command_pool,
    .level              = core::CommandBufferLevel::ePrimary,
    .commandBufferCount = 1,
  };

  auto buffers = raii::CommandBuffers(*get_gpu().logical, alloc_info);
  CommandBuffer cmd_copy_buffer {std::move(buffers.front())};

  CommandBuffer::begin_info begin_info {.flags = core::CommandBufferUsageFlagBits::eOneTimeSubmit};

  cmd_copy_buffer.record(begin_info, [dst_buffer, staging_buffer, copy_region](CommandBuffer const& cmd) {
    cmd.copy_buffer(staging_buffer, dst_buffer, copy_region);
  });

  SubmitInfo const submit_info = {.command_buffers = mono::as_span(**cmd_copy_buffer)};
  submit_work(submit_info, wait::idle);
}

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface) { //
  return get_gpu().physical.get_surface_info(surface);
}

} // namespace rf3d::gfx::vk::gpu
