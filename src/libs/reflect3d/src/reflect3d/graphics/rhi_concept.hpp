/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rhi_concept.hpp
 * @date 12/04/2026
 * @brief Render Hardware Interface concept
 *
 * All implementation traits must satisfy this concept
 *
 */
#pragma once

#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/api/fence.hpp"
#include "reflect3d/graphics/api/gpu.hpp"
#include "reflect3d/graphics/api/pso.hpp"
#include "reflect3d/graphics/api/surface.hpp"

namespace rf3d {

template<typename T>
concept RenderHardwareInterface = //
    Gpu<typename T::gpu> and //
    Fence<typename T::Fence> and //
    GraphicsCommandList<typename T::GraphicsCommandList> and //
    CopyCommandList<typename T::CopyCommandList> and //
    ComputeCommandList<typename T::ComputeCommandList> and //
    PipelineState<typename T::PipelineState> and //
    Surface<typename T::Surface> and //
    DrawCallDesc<typename T::DrawCall> and //
    VertexBuffer<typename T::VertexBuffer> and //
    IndexBuffer<typename T::IndexBuffer> and //
    UniformBuffer<typename T::UniformBuffer> and //
    StorageBuffer<typename T::StorageBuffer> and //
    ShaderResourceTexture<typename T::StaticTexture> and //
    ShaderResourceTexture<typename T::ComputeTexture> and //
    ShaderResourceTexture<typename T::ColorTexture> and //
    ShaderResourceTexture<typename T::RenderTargetTexture> and //
    ShaderResourceTexture<typename T::StorageTexture> and //
    RenderTargetTexture<typename T::RenderTargetTexture> and //
    DepthStencilTexture<typename T::DepthStencilTexture> and //
    CopySourceTexture<typename T::StaticTexture> and //
    CopyDestTexture<typename T::BackBufferTexture> and //
    CopyDestTexture<typename T::ColorTexture> and //
    UnorderedAccessTexture<typename T::ComputeTexture> and //
    UnorderedAccessTexture<typename T::StorageTexture>; //


} // namespace rf3d
