/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rhi_types.hpp
 * @date 26/05/2026
 * @brief RHI template type aliases and default backend type definitions
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/backends.hpp"
#include "reflect3d/graphics/rhi_concept.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::rhi {

// ---------------------------------------------------------------------------
// --- Template type traits (parameterized by RenderHardwareInterface) ---
// ---------------------------------------------------------------------------

// clang-format off
template<RenderHardwareInterface Rhi> using gpu_t = Rhi::gpu;
template<RenderHardwareInterface Rhi> using fence_t = Rhi::Fence;
template<RenderHardwareInterface Rhi> using semaphore_t = Rhi::Semaphore;
template<RenderHardwareInterface Rhi> using pipeline_state_t = Rhi::PipelineState;
template<RenderHardwareInterface Rhi> using surface_t = Rhi::Surface;
template<RenderHardwareInterface Rhi> using submit_info_t = Rhi::SubmitInfo;
template<RenderHardwareInterface Rhi> using frame_context_t = gpu_t<Rhi>::frame_context_type;

template<RenderHardwareInterface Rhi> using draw_call_t = Rhi::DrawCall;
template<RenderHardwareInterface Rhi> using graphics_command_list_t = Rhi::GraphicsCommandList;
template<RenderHardwareInterface Rhi> using compute_command_list_t = Rhi::ComputeCommandList;
template<RenderHardwareInterface Rhi> using copy_command_list_t = Rhi::CopyCommandList;
template<RenderHardwareInterface Rhi, DrawCallDesc DrawCallType = draw_call_t<Rhi>>
using draw_stream_builder_t = Rhi::template DrawStreamBuilder<DrawCallType>;

template<RenderHardwareInterface Rhi> using vertex_storage_buffer_t = Rhi::VertexStorageBuffer;
template<RenderHardwareInterface Rhi> using vertex_buffer_t = Rhi::VertexBuffer;
template<RenderHardwareInterface Rhi> using index_buffer_t = Rhi::IndexBuffer;
template<RenderHardwareInterface Rhi> using transfer_buffer_t = Rhi::TransferBuffer;
template<RenderHardwareInterface Rhi> using uniform_buffer_t = Rhi::UniformBuffer;
template<RenderHardwareInterface Rhi> using storage_buffer_t = Rhi::StorageBuffer;

template<RenderHardwareInterface Rhi> using static_texture_t = Rhi::StaticTexture;
template<RenderHardwareInterface Rhi> using compute_texture_t = Rhi::ComputeTexture;
template<RenderHardwareInterface Rhi> using back_buffer_texture_t = Rhi::BackBufferTexture;
template<RenderHardwareInterface Rhi> using color_texture_t = Rhi::ColorTexture;
template<RenderHardwareInterface Rhi> using depth_texture_t = Rhi::DepthTexture;
template<RenderHardwareInterface Rhi> using depth_stencil_texture_t = Rhi::DepthStencilTexture;
template<RenderHardwareInterface Rhi> using render_target_texture_t = Rhi::RenderTargetTexture;
template<RenderHardwareInterface Rhi> using storage_texture_t = Rhi::StorageTexture;
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
using DrawStreamBuilder   = impl::DefaultBackend::DrawStreamBuilder<DrawCallType>;
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
