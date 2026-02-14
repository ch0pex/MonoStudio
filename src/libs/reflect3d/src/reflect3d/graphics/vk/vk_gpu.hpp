#pragma once

#include "reflect3d/graphics/vk/gpu/vk_command_buffer.hpp"
#include "reflect3d/graphics/vk/gpu/vk_submit_info.hpp"
#include "reflect3d/graphics/vk/memory/vk_buffer_allocation.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_surface_info.hpp"

//
#include <mono/error/expected.hpp>


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

void submit_work(SubmitInfo const& submit_info, wait::fence_t wait);

void submit_work(SubmitInfo const& submit_info, wait::idle_t wait);

mono::err::expected<void> present(core::PresentInfoKHR const& present_info);

// -----------------------------
// --- Gpu factory functions ---
// -----------------------------

raii::ShaderModule make_shader_module(core::ShaderModuleCreateInfo const& module_info);

raii::ImageView make_image_view(core::ImageViewCreateInfo const& view_info);

raii::SwapchainKHR make_swapchain(core::SwapchainCreateInfoKHR const& swapchain_info);

raii::Semaphore make_semaphore(core::SemaphoreCreateInfo const& semaphore_info = {});

raii::Pipeline make_graphics_pipeline(core::GraphicsPipelineCreateInfo const& pipeline_info);

raii::PipelineLayout make_pipeline_layout(core::PipelineLayoutCreateInfo const& layout_info);


// -------------------------------------
// --- Gpu memory resource functions ---
// -------------------------------------

BufferAllocation allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info = {});

void free_buffer(BufferAllocation const& buffer);

void upload_buffer(core::Buffer dst_buffer, core::Buffer staging_buffer, core::BufferCopy const& copy_region);

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface);

} // namespace rf3d::gfx::vk::gpu
