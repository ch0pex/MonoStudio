
// Archetypes
#include <reflect3d/graphics/api/archetypes/buffer.hpp>
#include <reflect3d/graphics/api/archetypes/command_list.hpp>
#include <reflect3d/graphics/api/archetypes/fence.hpp>
#include <reflect3d/graphics/api/archetypes/gpu.hpp>
#include <reflect3d/graphics/api/archetypes/pso.hpp>
#include <reflect3d/graphics/api/archetypes/renderpass.hpp>
#include <reflect3d/graphics/api/archetypes/semaphore.hpp>
#include <reflect3d/graphics/api/archetypes/surface.hpp>
#include <reflect3d/graphics/api/archetypes/texture.hpp>

// Concepts
#include <reflect3d/graphics/api/buffer.hpp>
#include <reflect3d/graphics/api/command_list.hpp>
#include <reflect3d/graphics/api/fence.hpp>
#include <reflect3d/graphics/api/gpu.hpp>
#include <reflect3d/graphics/api/pso.hpp>
#include <reflect3d/graphics/api/renderpass.hpp>
#include <reflect3d/graphics/api/semaphore.hpp>
#include <reflect3d/graphics/api/surface.hpp>
#include <reflect3d/graphics/api/texture.hpp>

#include "reflect3d/graphics/rhi.hpp"

using namespace rf3d;

namespace {

// Buffer concepts
static_assert(Buffer<archetypes::DedicatedBuffer>);
static_assert(Buffer<archetypes::MappedBuffer>);
static_assert(Buffer<archetypes::SourceBuffer>);
static_assert(Buffer<archetypes::DestinationBuffer>);
static_assert(Buffer<archetypes::UniformBuffer>);
static_assert(Buffer<archetypes::StorageBuffer>);
static_assert(Buffer<archetypes::VertexBuffer>);
static_assert(Buffer<archetypes::IndexBuffer>);
static_assert(DedicatedBuffer<archetypes::DedicatedBuffer>);
static_assert(MappedBuffer<archetypes::MappedBuffer>);
static_assert(SourceBuffer<archetypes::SourceBuffer>);
static_assert(DestinationBuffer<archetypes::DestinationBuffer>);
static_assert(UniformBuffer<archetypes::UniformBuffer>);
static_assert(StorageBuffer<archetypes::StorageBuffer>);
static_assert(VertexBuffer<archetypes::VertexBuffer>);
static_assert(IndexBuffer<archetypes::IndexBuffer>);

// Texture concepts
static_assert(Texture<archetypes::ColorTexture>);
static_assert(Texture<archetypes::RenderTargetTexture>);
static_assert(Texture<archetypes::DepthTexture>);
static_assert(Texture<archetypes::DepthStencilTexture>);
static_assert(Texture<archetypes::StorageTexture>);
static_assert(Texture<archetypes::CopySourceTexture>);
static_assert(Texture<archetypes::CopyDestTexture>);
static_assert(ShaderResourceTexture<archetypes::ColorTexture>);
static_assert(RenderTargetTexture<archetypes::RenderTargetTexture>);
static_assert(DepthStencilTexture<archetypes::DepthTexture>);
static_assert(DepthStencilTexture<archetypes::DepthStencilTexture>);
static_assert(UnorderedAccessTexture<archetypes::StorageTexture>);
static_assert(CopySourceTexture<archetypes::CopySourceTexture>);
static_assert(CopyDestTexture<archetypes::CopyDestTexture>);

// PipelineState concept
static_assert(PipelineState<archetypes::PipelineState>);

// CommandList concepts
static_assert(CommandListLifetime<archetypes::CommandList>);
static_assert(CommandListPass<archetypes::CommandList>);
static_assert(CommandListSynchronization<archetypes::CommandList>);
static_assert(CommandListBinding<archetypes::CommandList>);
static_assert(CommandListState<archetypes::CommandList>);
static_assert(CommandListPushConstants<archetypes::CommandList>);
static_assert(CommandListTransfer<archetypes::CommandList>);
static_assert(CommandListCompute<archetypes::CommandList>);
static_assert(CommandListDraw<archetypes::CommandList>);
static_assert(CopyCommandList<archetypes::CommandList>);
static_assert(ComputeCommandList<archetypes::CommandList>);
static_assert(GraphicsCommandList<archetypes::CommandList>);
static_assert(CommandList<archetypes::CommandList>);

// Fence concept
static_assert(Fence<archetypes::Fence>);

// Semaphore concept
static_assert(Semaphore<archetypes::Semaphore>);

// Gpu concept
static_assert(Gpu<archetypes::Gpu>);

// Surface concept
static_assert(Surface<archetypes::Surface>);

// RenderPass concepts
static_assert(ColorTargetDesc<archetypes::ColorTarget<>>);
static_assert(DepthTargetDesc<archetypes::DepthTarget<>>);
static_assert(RenderTargetDesc<archetypes::ColorTarget<>>);
static_assert(RenderTargetDesc<archetypes::DepthTarget<>>);
static_assert(DrawCallDesc<archetypes::DrawCall<>>);
static_assert(DrawAreaDesc<archetypes::DrawArea<>>);
static_assert(RenderPassDesc<archetypes::RenderPass<>>);

// Render Hardware Interfaces
static_assert(RenderHardwareInterface<impl::vk>, "VulkanTraits does not satisfy the RHI concept");


} // namespace
