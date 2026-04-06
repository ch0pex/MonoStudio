#pragma once

#include "reflect3d/graphics/core/command_list_type.hpp"
#include "reflect3d/graphics/vk/detail/gpu/vk_submit_info.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_buffer_allocation.hpp"
#include "reflect3d/graphics/vk/detail/vk_surface_info.hpp"

//
#include <mono/containers/span.hpp>
#include <mono/error/expected.hpp>
//
#include <chrono>


namespace rf3d::vk::detail {

// ----------------------------------------------
// --- Gpu frame commands and synchronization ---
// ----------------------------------------------

void wait_idle();

void wait_fence(core::Fence fences, std::chrono::nanoseconds timeout);

void wait_fences(mono::span<core::Fence> fences, bool wait_all, std::chrono::nanoseconds timeout);

void reset_fences(mono::span<core::Fence const> fences);

void wait_semaphores(
    mono::span<core::Semaphore const> semaphore, //
    mono::span<std::uint64_t const> wait_values, //
    std::chrono::milliseconds timeout //
);

void wait_semaphore(core::Semaphore semaphore, std::uint64_t value, std::chrono::nanoseconds timeout);

void signal_semaphore(core::Semaphore semaphore, std::uint64_t signalValue);

void submit_work(SubmitInfo const& submit_info, core::Fence fence);

void submit_work(SubmitInfo const& submit_info);

mono::expected<void> present(core::PresentInfoKHR const& present_info);

// -----------------------------
// --- Gpu factory functions ---
// -----------------------------

raii::ShaderModule make_shader_module(core::ShaderModuleCreateInfo const& module_info);

raii::ImageView make_image_view(core::ImageViewCreateInfo const& view_info);

raii::SwapchainKHR make_swapchain(core::SwapchainCreateInfoKHR const& swapchain_info);

raii::Semaphore make_semaphore(core::SemaphoreCreateInfo const& semaphore_info = {});

raii::Pipeline make_graphics_pipeline(core::GraphicsPipelineCreateInfo const& pipeline_info);

raii::PipelineLayout make_pipeline_layout(core::PipelineLayoutCreateInfo const& layout_info);

raii::Fence make_fence(core::FenceCreateInfo const& fence_info = {.flags = core::FenceCreateFlagBits::eSignaled});

// -------------------------------------
// --- Gpu memory resource functions ---
// -------------------------------------

raii::CommandBuffers allocate_command_buffer(CommandListType level, std::uint32_t count);

BufferAllocation allocate_buffer(core::BufferCreateInfo const& buf_info, AllocationCreateInfo const& alloc_info = {});

void free_buffer(BufferAllocation const& buffer);

void upload_buffer(core::Buffer dst_buffer, core::Buffer staging_buffer, core::BufferCopy const& copy_region);

// --------------------------------
// --- Gpu info query functions ---
// --------------------------------

SurfaceInfo get_surface_info(raii::SurfaceKHR const& surface);

} // namespace rf3d::vk::detail
