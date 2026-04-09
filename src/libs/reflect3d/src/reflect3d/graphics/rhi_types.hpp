#pragma once

#include "reflect3d/graphics/backends.hpp"
#include "reflect3d/graphics/rhi_concept.hpp"

namespace rf3d::rhi {

// ---------------------------------------------------------------------------
// --- Template type traits (parameterized by RenderHardwareInterface) ---
// ---------------------------------------------------------------------------

// clang-format off
template<RenderHardwareInterface Rhi> using gpu_t = typename Rhi::gpu;
template<RenderHardwareInterface Rhi> using fence_t = typename Rhi::Fence;
template<RenderHardwareInterface Rhi> using semaphore_t = typename Rhi::Semaphore;
template<RenderHardwareInterface Rhi> using pipeline_state_t = typename Rhi::PipelineState;
template<RenderHardwareInterface Rhi> using surface_t = typename Rhi::Surface;
template<RenderHardwareInterface Rhi> using submit_info_t = typename Rhi::SubmitInfo;
template<RenderHardwareInterface Rhi> using frame_context_t = typename gpu_t<Rhi>::frame_context_type;

template<RenderHardwareInterface Rhi> using draw_call_t = typename Rhi::DrawCall;
template<RenderHardwareInterface Rhi> using graphics_command_list_t = typename Rhi::GraphicsCommandList;
template<RenderHardwareInterface Rhi> using compute_command_list_t = typename Rhi::ComputeCommandList;
template<RenderHardwareInterface Rhi> using copy_command_list_t = typename Rhi::CopyCommandList;
template<RenderHardwareInterface Rhi, DrawCallDesc DrawCallType = draw_call_t<Rhi>>
using draw_stream_builder_t = typename Rhi::template DrawStreamBuilder<DrawCallType>;

template<RenderHardwareInterface Rhi> using vertex_storage_buffer_t = typename Rhi::VertexStorageBuffer;
template<RenderHardwareInterface Rhi> using vertex_buffer_t = typename Rhi::VertexBuffer;
template<RenderHardwareInterface Rhi> using index_buffer_t = typename Rhi::IndexBuffer;
template<RenderHardwareInterface Rhi> using transfer_buffer_t = typename Rhi::TransferBuffer;
template<RenderHardwareInterface Rhi> using uniform_buffer_t = typename Rhi::UniformBuffer;
template<RenderHardwareInterface Rhi> using storage_buffer_t = typename Rhi::StorageBuffer;

template<RenderHardwareInterface Rhi> using static_texture_t = typename Rhi::StaticTexture;
template<RenderHardwareInterface Rhi> using compute_texture_t = typename Rhi::ComputeTexture;
template<RenderHardwareInterface Rhi> using back_buffer_texture_t = typename Rhi::BackBufferTexture;
template<RenderHardwareInterface Rhi> using color_texture_t = typename Rhi::ColorTexture;
template<RenderHardwareInterface Rhi> using depth_texture_t = typename Rhi::DepthTexture;
template<RenderHardwareInterface Rhi> using depth_stencil_texture_t = typename Rhi::DepthStencilTexture;
template<RenderHardwareInterface Rhi> using render_target_texture_t = typename Rhi::RenderTargetTexture;
template<RenderHardwareInterface Rhi> using storage_texture_t = typename Rhi::StorageTexture;
// clang-format on

// -------------------------------------------------
// --- Direct type aliases (DefaultBackend) ---
// -------------------------------------------------

using gpu           = gpu_t<impl::DefaultBackend>;
using Fence         = fence_t<impl::DefaultBackend>;
using Semaphore     = semaphore_t<impl::DefaultBackend>;
using PipelineState = pipeline_state_t<impl::DefaultBackend>;
using Surface       = surface_t<impl::DefaultBackend>;
using SubmitInfo    = submit_info_t<impl::DefaultBackend>;
using FrameContext  = frame_context_t<impl::DefaultBackend>;

template<DrawCallDesc DrawCallType = impl::DefaultBackend::DrawCall>
using DrawStreamBuilder   = typename impl::DefaultBackend::DrawStreamBuilder<DrawCallType>;
using DrawCall            = draw_call_t<impl::DefaultBackend>;
using GraphicsCommandList = graphics_command_list_t<impl::DefaultBackend>;
using ComputeCommandList  = compute_command_list_t<impl::DefaultBackend>;
using CopyCommandList     = copy_command_list_t<impl::DefaultBackend>;

using VertexStorageBuffer = vertex_storage_buffer_t<impl::DefaultBackend>;
using VertexBuffer        = vertex_buffer_t<impl::DefaultBackend>;
using IndexBuffer         = index_buffer_t<impl::DefaultBackend>;
using TransferBuffer      = transfer_buffer_t<impl::DefaultBackend>;
using UniformBuffer       = uniform_buffer_t<impl::DefaultBackend>;
using StorageBuffer       = storage_buffer_t<impl::DefaultBackend>;

using StaticTexture       = static_texture_t<impl::DefaultBackend>;
using ComputeTexture      = compute_texture_t<impl::DefaultBackend>;
using BackBufferTexture   = back_buffer_texture_t<impl::DefaultBackend>;
using ColorTexture        = color_texture_t<impl::DefaultBackend>;
using DepthTexture        = depth_texture_t<impl::DefaultBackend>;
using DepthStencilTexture = depth_stencil_texture_t<impl::DefaultBackend>;
using RenderTargetTexture = render_target_texture_t<impl::DefaultBackend>;
using StorageTexture      = storage_texture_t<impl::DefaultBackend>;


} // namespace rf3d::rhi
