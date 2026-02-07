#pragma once

#include "reflect3d/graphics/vk/gpu/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_surface_info.hpp"

//

#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>

//
#include <assets_path.hpp>

//
#include <cassert>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace rf3d::gfx::vk::gpu {

// ----------------------------------------------
// --- Gpu frame commands and synchronization ---
// ----------------------------------------------

void wait_idle();

FrameIndex next_frame();

core::CommandBuffer record_commands(
    CommandBuffer::begin_info const& begin_info, //
    std::function<void(CommandBuffer const&)> const& command_recording_func
);

void submit_work(
    std::span<core::Semaphore const> wait_semaphores, //
    std::span<core::CommandBuffer const> command_buffers, //
    std::span<core::Semaphore const> signal_semaphores //
);

mono::err::expected<void> present(core::PresentInfoKHR const& present_info);

// --------------------------------------
// --- Gpu resource factory functions ---
// --------------------------------------

raii::ShaderModule make_shader_module(core::ShaderModuleCreateInfo const& module_info);

raii::ImageView make_image_view(core::ImageViewCreateInfo const& view_info);

raii::SwapchainKHR make_swapchain(core::SwapchainCreateInfoKHR const& swapchain_info);

raii::Semaphore make_semaphore(core::SemaphoreCreateInfo const& semaphore_info = {});

raii::Pipeline make_graphics_pipeline(core::GraphicsPipelineCreateInfo const& pipeline_info);

raii::PipelineLayout make_pipeline_layout(core::PipelineLayoutCreateInfo const& layout_info);

raii::Buffer make_buffer(core::BufferCreateInfo const& buffer_info);

raii::DeviceMemory allocate_memory(core::MemoryRequirements const& req, core::MemoryPropertyFlags const& prop);

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------


core::PhysicalDeviceMemoryProperties get_memory_properties();

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface);

} // namespace rf3d::gfx::vk::gpu
