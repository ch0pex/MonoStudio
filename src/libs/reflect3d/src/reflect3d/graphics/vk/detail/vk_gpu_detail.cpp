
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"
#include <chrono>

#include "mono/misc/as_span.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_command_pool.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_logical_device.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_physical_device.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_memory_allocator.hpp"
#include "reflect3d/graphics/vk/detail/vk_instance.hpp"


namespace rf3d::vk::detail {

namespace {

struct Gpu {
  Gpu(PhysicalDevice&& physical_device) :
    physical(std::move(physical_device)), //
    logical(physical.create_logical_device()), //
    memory_allocator(create_allocator(*physical, *logical)), //
    queues(logical, physical.queue_creation_info()), //
    command_pools(logical, physical) { }

  PhysicalDevice physical;
  LogicalDevice logical;
  MemoryAllocator memory_allocator;
  Queues<GraphicQueue, PresentQueue> queues;
  CommandPools command_pools;
};

Gpu& get_gpu() {
  static Gpu instance {pick_best_physical_device()};
  return instance;
}

} // namespace

// ---------------------------------------------------------------------------------------------
// --- Gpu frame commands and synchronization --------------------------------------------------
// ---------------------------------------------------------------------------------------------

void wait_idle() { //
  // LOG_INFO("Waiting for GPU to be idle...");
  get_gpu().logical.wait_idle();
}

void wait_fence(core::Fence const fence, std::chrono::nanoseconds const timeout) {
  get_gpu().logical.wait_fence(fence, true, timeout);
}

void wait_fences(mono::span<core::Fence> fences, bool const wait_all, std::chrono::nanoseconds const timeout) {
  get_gpu().logical.wait_for_fences(fences, wait_all, timeout);
}

void reset_fences(mono::span<core::Fence const> fences) { //
  get_gpu().logical.reset_fences(fences);
}

void wait_semaphores(
    mono::span<core::Semaphore const> semaphore, //
    mono::span<std::uint64_t const> wait_values, //
    std::chrono::milliseconds timeout //
) {
  get_gpu().logical.wait_semaphores(semaphore, wait_values, timeout);
}

void wait_semaphore(core::Semaphore const semaphore, std::uint64_t const value, std::chrono::milliseconds timeout) {
  wait_semaphores({semaphore}, {value}, timeout);
}

void signal_semaphore(core::Semaphore const& semaphore, std::uint64_t const signalValue) {
  get_gpu().logical.signal_semaphore(semaphore, signalValue);
}

void submit_work(SubmitInfo const& submit_info, core::Fence const fence) {
  get_gpu().queues.graphics().submit(to_native(submit_info), fence);
}

void submit_work(SubmitInfo const& submit_info) { //
  get_gpu().queues.graphics().submit(to_native(submit_info));
}

mono::expected<void> present(core::PresentInfoKHR const& present_info) {
  auto const result = get_gpu().queues.present().presentKHR(present_info);
  if (result == core::Result::eSuboptimalKHR or result == core::Result::eErrorOutOfDateKHR) {
    return mono::unexpected("Swapchain is out of date or suboptimal");
  }
  assert(result == core::Result::eSuccess);
  return mono::expected<void> {};
}

// ----------------------------------------------------------------------------
// --- Gpu factory functions --------------------------------------------------
// ----------------------------------------------------------------------------

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

raii::Fence make_fence(core::FenceCreateInfo const& fence_info) { //
  return {*get_gpu().logical, fence_info};
}

// ------------------------------------------------------------------------------------
// --- Gpu memory resource functions --------------------------------------------------
// ------------------------------------------------------------------------------------

// NOTE: monitor using transient command pool performance
raii::CommandBuffers allocate_command_buffer(CommandListType const level, std::uint32_t const count) {
  switch (level) {
    case CommandListType::graphics:
      return get_gpu().command_pools.allocate_primary_buffers(count);
    case CommandListType::compute:
    case CommandListType::copy:
      return get_gpu().command_pools.allocate_transient_buffers(count);
    default:
      throw std::runtime_error {"Unsupported command list type"};
  }
}

BufferAllocation allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info) {
  return get_gpu().memory_allocator.allocate_buffer(buf_info, alloc_info);
}

void free_buffer(BufferAllocation const& buffer_allocation) {
  get_gpu().memory_allocator.free_buffer(buffer_allocation);
}

// -------------------------------------------------------------------------------
// --- Gpu info query functions --------------------------------------------------
// -------------------------------------------------------------------------------

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface) { //
  return get_gpu().physical.get_surface_info(surface);
}


} // namespace rf3d::vk::detail
