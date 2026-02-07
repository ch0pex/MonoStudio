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
#include <mono/misc/passkey.hpp>


//
#include <assets_path.hpp>

//
#include <cassert>
#include <vulkan/vulkan_handles.hpp>

namespace rf3d::gfx::vk::gpu {

namespace {

std::uint64_t rate_device(raii::PhysicalDevice const& device) {
  auto const device_properties = device.getProperties();
  auto const device_features   = device.getFeatures();

  std::uint64_t score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == core::PhysicalDeviceType::eDiscreteGpu) {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += device_properties.limits.maxImageDimension2D;
  score += device_properties.limits.maxImageDimension3D;
  score += device_properties.limits.maxImageDimensionCube;
  score += device_properties.limits.maxComputeSharedMemorySize;

  // Application can't function without geometry shaders
  if (device_features.geometryShader == VK_FALSE) {
    return 0;
  }

  return score;
}

PhysicalDevice pick_best_physical_device() {
  LOG_INFO("Picking the best GPU for the application");
  std::multimap<std::uint64_t, raii::PhysicalDevice, std::greater<>> candidates;

  for (auto const& device: instance::physical_devices()) {
    LOG_INFO("Found GPU: {}", std::string_view {device.getProperties().deviceName});
    candidates.insert({rate_device(device), device});
  }

  if (candidates.empty() or candidates.begin()->first == 0) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  return PhysicalDevice {candidates.begin()->second};
}

std::uint32_t find_memory_type(std::uint32_t type_filter, core::MemoryPropertyFlags properties) {
  auto const mem_properties = gpu::get_memory_properties();

  for (std::uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
    bool const filters_match    = (type_filter & (1 << i)) != 0U;
    bool const properties_match = (mem_properties.memoryTypes.at(i).propertyFlags & properties) == properties;
    if (filters_match and properties_match) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type");
}

struct Gpu {
  PhysicalDevice physical {pick_best_physical_device()};
  LogicalDevice logical {physical.create_logical_device()};
  Queues<GraphicQueue, PresentQueue> queues {logical, physical.queue_creation_info()};
  CommandPool command_pool {
    logical, //
    CommandPool::config_type {
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

raii::Buffer make_buffer(core::BufferCreateInfo const& buffer_info) { //
  return {*get_gpu().logical, buffer_info};
}


raii::DeviceMemory allocate_memory(core::MemoryRequirements const& req, core::MemoryPropertyFlags const& prop) {
  auto memory_alloc_info = core::MemoryAllocateInfo {
    .allocationSize  = req.size,
    .memoryTypeIndex = find_memory_type(req.memoryTypeBits, prop),
  };
  return {*get_gpu().logical, memory_alloc_info};
}

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------

core::PhysicalDeviceMemoryProperties get_memory_properties() { //
  return get_gpu().physical.get_memory_properties();
}

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface) { //
  return get_gpu().physical.get_surface_info(surface);
}

} // namespace rf3d::gfx::vk::gpu
