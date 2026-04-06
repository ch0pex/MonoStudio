#include <reflect3d/graphics/api/command_list.hpp>
#include <reflect3d/graphics/api/pso.hpp>
#include <reflect3d/graphics/api/texture.hpp>
#include <reflect3d/graphics/vk/command_list.hpp>
#include <reflect3d/graphics/vk/fence.hpp>
#include <reflect3d/graphics/vk/gpu.hpp>
#include <reflect3d/graphics/vk/pso.hpp>
#include <reflect3d/graphics/vk/surface.hpp>
#include <reflect3d/graphics/vk/texture.hpp>

#include "reflect3d/graphics/api/fence.hpp"
#include "reflect3d/graphics/api/gpu.hpp"
#include "reflect3d/graphics/api/surface.hpp"

namespace {

// Texture concepts
static_assert(rf3d::Texture<rf3d::vk::ColorTexture>);
static_assert(rf3d::Texture<rf3d::vk::RenderTargetTexture>);
static_assert(rf3d::Texture<rf3d::vk::DepthTexture>);
static_assert(rf3d::Texture<rf3d::vk::DepthStencilTexture>);
static_assert(rf3d::Texture<rf3d::vk::StorageTexture>);
static_assert(rf3d::Texture<rf3d::vk::StaticTexture>);
static_assert(rf3d::ShaderResourceTexture<rf3d::vk::ColorTexture>);
static_assert(rf3d::RenderTargetTexture<rf3d::vk::RenderTargetTexture>);
static_assert(rf3d::DepthStencilTexture<rf3d::vk::DepthTexture>);
static_assert(rf3d::DepthStencilTexture<rf3d::vk::DepthStencilTexture>);
static_assert(rf3d::UnorderedAccessTexture<rf3d::vk::StorageTexture>);
static_assert(rf3d::CopySourceTexture<rf3d::vk::StaticTexture>);
static_assert(rf3d::CopyDestTexture<rf3d::vk::StaticTexture>);

// PipelineState concept
static_assert(rf3d::PipelineState<rf3d::vk::PipelineState>);

// CommandList concepts
// static_assert(rf3d::CopyCommandList<rf3d::vk::GraphicsCommandList>);
// static_assert(rf3d::ComputeCommandList<rf3d::vk::GraphicsCommandList>);
// static_assert(rf3d::GraphicsCommandList<rf3d::vk::GraphicsCommandList>);

// // Fence concept
static_assert(rf3d::Fence<rf3d::vk::Fence>);
//
// // Gpu concept
static_assert(rf3d::Gpu<rf3d::vk::Gpu>);
//
// // Surface concept
// static_assert(rf3d::Surface<rf3d::vk::Surface>);

} // namespace
