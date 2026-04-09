/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file backends.hpp
 * @version 1.0
 * @date 11/04/2026
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include "reflect3d/graphics/vk/buffer.hpp"
#include "reflect3d/graphics/vk/command_list.hpp"
#include "reflect3d/graphics/vk/draw_stream.hpp"
#include "reflect3d/graphics/vk/fence.hpp"
#include "reflect3d/graphics/vk/gpu.hpp"
#include "reflect3d/graphics/vk/pso.hpp"
#include "reflect3d/graphics/vk/surface.hpp"
#include "reflect3d/graphics/vk/texture.hpp"

namespace rf3d::impl {

struct vk {
  using gpu           = rf3d::vk::Gpu;
  using Fence         = rf3d::vk::Fence;
  using Semaphore     = rf3d::vk::Semaphore;
  using PipelineState = rf3d::vk::PipelineState;
  using Surface       = rf3d::vk::Surface;
  using SubmitInfo    = rf3d::vk::SubmitInfo;

  template<typename DrawCallType = rf3d::vk::DrawCall>
  using DrawStreamBuilder   = rf3d::vk::DrawStreamBuilder<DrawCallType>;
  using DrawCall            = rf3d::vk::DrawCall;
  using GraphicsCommandList = rf3d::vk::GraphicsCommandList;
  using ComputeCommandList  = rf3d::vk::ComputeCommandList;
  using CopyCommandList     = rf3d::vk::CopyCommandList;

  template<BufferUsage Usage, MemoryProperty Mem>
  using Buffer              = rf3d::vk::Buffer<Usage, Mem>;
  using VertexStorageBuffer = rf3d::vk::VertexStorageBuffer;
  using VertexBuffer        = rf3d::vk::VertexBuffer;
  using IndexBuffer         = rf3d::vk::IndexBuffer;
  using TransferBuffer      = rf3d::vk::TransferBuffer;
  using UniformBuffer       = rf3d::vk::UniformBuffer;
  using StorageBuffer       = rf3d::vk::StorageBuffer;

  template<TextureUsage Usage>
  using Texture             = rf3d::vk::Texture<Usage>;
  using StaticTexture       = rf3d::vk::StaticTexture;
  using ComputeTexture      = rf3d::vk::ComputeTexture;
  using BackBufferTexture   = rf3d::vk::BackBufferTexture;
  using ColorTexture        = rf3d::vk::ColorTexture;
  using DepthTexture        = rf3d::vk::DepthTexture;
  using DepthStencilTexture = rf3d::vk::DepthStencilTexture;
  using RenderTargetTexture = rf3d::vk::RenderTargetTexture;
  using StorageTexture      = rf3d::vk::StorageTexture;
};

#if defined(RF3D_SUPPORT_DX12)
struct dx12 {
  using gpu           = rf3d::dx12::Gpu;
  using Fence         = rf3d::dx12::Fence;
  using Semaphore     = rf3d::dx12::Semaphore;
  using PipelineState = rf3d::dx12::PipelineState;
  using Surface       = rf3d::dx12::Surface;

  template<typename DrawCallType = rf3d::dx12::DrawCall>
  using DrawStreamBuilder   = rf3d::dx12::DrawStreamBuilder<DrawCallType>;
  using GraphicsCommandList = rf3d::dx12::GraphicsCommandList;
  using ComputeCommandList  = rf3d::dx12::ComputeCommandList;
  using CopyCommandList     = rf3d::dx12::CopyCommandList;

  template<BufferUsage Usage, MemoryProperty Mem>
  using Buffer              = rf3d::dx12::Buffer<Usage, Mem>;
  using VertexStorageBuffer = rf3d::dx12::VertexStorageBuffer;
  using VertexBuffer        = rf3d::dx12::VertexBuffer;
  using IndexBuffer         = rf3d::dx12::IndexBuffer;
  using TransferBuffer      = rf3d::dx12::TransferBuffer;
  using UniformBuffer       = rf3d::dx12::UniformBuffer;
  using StorageBuffer       = rf3d::dx12::StorageBuffer;

  template<TextureUsage Usage>
  using Texture             = rf3d::dx12::Texture<Usage>;
  using StaticTexture       = rf3d::dx12::StaticTexture;
  using ComputeTexture      = rf3d::dx12::ComputeTexture;
  using BackBufferTexture   = rf3d::dx12::BackBufferTexture;
  using ColorTexture        = rf3d::dx12::ColorTexture;
  using DepthTexture        = rf3d::dx12::DepthTexture;
  using DepthStencilTexture = rf3d::dx12::DepthStencilTexture;
  using RenderTargetTexture = rf3d::dx12::RenderTargetTexture;
  using StorageTexture      = rf3d::dx12::StorageTexture;
};
#endif

#if defined(RF3D_SUPPORT_METAL)
struct metal {
  using gpu           = rf3d::metal::Gpu;
  using Fence         = rf3d::metal::Fence;
  using Semaphore     = rf3d::metal::Semaphore;
  using PipelineState = rf3d::metal::PipelineState;
  using Surface       = rf3d::metal::Surface;

  template<typename DrawCallType = rf3d::metal::DrawCall>
  using DrawStreamBuilder   = rf3d::metal::DrawStreamBuilder<DrawCallType>;
  using GraphicsCommandList = rf3d::metal::GraphicsCommandList;
  using ComputeCommandList  = rf3d::metal::ComputeCommandList;
  using CopyCommandList     = rf3d::metal::CopyCommandList;

  template<BufferUsage Usage, MemoryProperty Mem>
  using Buffer              = rf3d::metal::Buffer<Usage, Mem>;
  using VertexStorageBuffer = rf3d::metal::VertexStorageBuffer;
  using VertexBuffer        = rf3d::metal::VertexBuffer;
  using IndexBuffer         = rf3d::metal::IndexBuffer;
  using TransferBuffer      = rf3d::metal::TransferBuffer;
  using UniformBuffer       = rf3d::metal::UniformBuffer;
  using StorageBuffer       = rf3d::metal::StorageBuffer;

  template<TextureUsage Usage>
  using Texture             = rf3d::metal::Texture<Usage>;
  using StaticTexture       = rf3d::metal::StaticTexture;
  using ComputeTexture      = rf3d::metal::ComputeTexture;
  using BackBufferTexture   = rf3d::metal::BackBufferTexture;
  using ColorTexture        = rf3d::metal::ColorTexture;
  using DepthTexture        = rf3d::metal::DepthTexture;
  using DepthStencilTexture = rf3d::metal::DepthStencilTexture;
  using RenderTargetTexture = rf3d::metal::RenderTargetTexture;
  using StorageTexture      = rf3d::metal::StorageTexture;
};
#endif

#if defined(RF3D_VULKAN_DEFAULT)
using DefaultBackend = vk;
#elif defined(RF3D_SUPPORT_DX12) && defined(RF3D_DX12_DEFAULT)
#error "DirectX 12 backend is not implemented yet"
#elif defined(RF3D_SUPPORT_METAL) && defined(RF3D_METAL_DEFAULT)
#error "Metal backend is not implemented yet"
#endif

} // namespace rf3d::impl
