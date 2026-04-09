#pragma once

#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/renderpass.hpp"
#include "reflect3d/graphics/api/texture.hpp"
#include "reflect3d/graphics/core/command_list_type.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/core/renderpass_descriptor.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/viewport.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_to_native.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"
#include "reflect3d/graphics/vk/pso.hpp"

#include <mono/containers/static_vector.hpp>
#include <mono/containers/tuple.hpp>
#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>

#include <cstdint>

namespace rf3d::vk {


template<CommandListType Type = graphics>
class CommandList {
public:
  // --- Type Traits ---

  using native_type = detail::raii::CommandBuffer;
  using list_type   = std::integral_constant<CommandListType, Type>;
  using pso_type    = PipelineState;

  // --- Constructors ---

  explicit CommandList() : cmd_buffer(std::move(detail::allocate_command_buffer(list_type::value, 1).front())) {
    LOG_INFO("Allocated command buffer for list type {}", static_cast<std::uint32_t>(list_type::value));
  }

  // --- Common commands ---

  CommandList& reset() {
    cmd_buffer.reset();
    return *this;
  }

  CommandList& begin() {
    cmd_buffer.begin({});
    return *this;
  }

  CommandList& end() {
    flush_barriers();
    cmd_buffer.end();
    return *this;
  }

  CommandList& bind_pipeline(pso_type const& pso [[maybe_unused]]) {
    cmd_buffer.bindPipeline(detail::core::PipelineBindPoint::eGraphics, *pso.handle());
    return *this;
  }

  CommandList&
  push_constants(mono::span<std::byte const> bytes [[maybe_unused]]) { // TODO: add push constant parameters
    // cmd_buffer.pushConstants(jkvk::PipelineLayout layout, vk::ShaderStageFlags stageFlags, uint32_t offset, const
    return *this;
  }

  CommandList& barrier(Texture auto& texture, ResourceState const new_state, bool flush = false) {
    if (new_state == texture.current_state()) {
      return *this;
    }

    auto src = detail::to_native(flush ? ResourceState::undefined : texture.current_state());
    auto dst = detail::to_native(new_state);

    auto aspect_mask = (new_state == ResourceState::depth_write) //
                           ? detail::core::ImageAspectFlagBits::eDepth
                           : detail::core::ImageAspectFlagBits::eColor;

    pending_image_barriers.emplace_back(
        detail::core::ImageMemoryBarrier2 {
          .srcStageMask        = src.stage_mask,
          .srcAccessMask       = src.access_mask,
          .dstStageMask        = dst.stage_mask,
          .dstAccessMask       = dst.access_mask,
          .oldLayout           = src.image_layout,
          .newLayout           = dst.image_layout,
          .srcQueueFamilyIndex = detail::core::QueueFamilyIgnored,
          .dstQueueFamilyIndex = detail::core::QueueFamilyIgnored,
          .image               = texture.view().gpu_handle,
          .subresourceRange    = {
               .aspectMask     = aspect_mask,
               .baseMipLevel   = 0,
               .levelCount     = detail::core::RemainingMipLevels,
               .baseArrayLayer = 0,
               .layerCount     = detail::core::RemainingArrayLayers,
          }
        }
    );

    texture.set_state(new_state, mono::PassKey<CommandList<Type>> {});
    return *this;
  }

  CommandList& barrier(rf3d::DedicatedBuffer auto& buffer, ResourceState const new_state) {
    if (new_state == buffer.current_state()) {
      return *this;
    }

    auto src = detail::to_native(buffer.current_state());
    auto dst = detail::to_native(new_state);

    pending_buffer_barriers.emplace_back(
        detail::core::BufferMemoryBarrier2 {
          .srcStageMask        = src.stage_mask,
          .srcAccessMask       = src.access_mask,
          .dstStageMask        = dst.stage_mask,
          .dstAccessMask       = dst.access_mask,
          .srcQueueFamilyIndex = detail::core::QueueFamilyIgnored,
          .dstQueueFamilyIndex = detail::core::QueueFamilyIgnored,
          .buffer              = buffer.handle().buffer_handle,
          .offset              = 0,
          .size                = detail::core::WholeSize
        }
    );

    buffer.set_state(new_state, mono::PassKey<CommandList<Type>> {});
    return *this;
  }

  CommandList& flush_barriers() {
    detail::core::DependencyInfo dependency_info {
      .bufferMemoryBarrierCount = static_cast<std::uint32_t>(pending_buffer_barriers.size()),
      .pBufferMemoryBarriers    = pending_buffer_barriers.data(),
      .imageMemoryBarrierCount  = static_cast<std::uint32_t>(pending_image_barriers.size()),
      .pImageMemoryBarriers     = pending_image_barriers.data(),
    };

    pending_buffer_barriers.clear();
    pending_image_barriers.clear();
    cmd_buffer.pipelineBarrier2(dependency_info);
    return *this;
  }

  CommandList& copy_buffer(SourceBuffer auto& src, DestinationBuffer auto& dst, BufferCopyRegion const& copy_region) {
    detail::core::BufferCopy const region {
      .srcOffset = copy_region.src_offset,
      .dstOffset = copy_region.dst_offset,
      .size      = copy_region.size,
    };
    cmd_buffer.copyBuffer(src.handle(), dst.handle(), region);
    return *this;
  }

  CommandList& copy_texture(CopySourceTexture auto& src, CopyDestTexture auto& dst) {
    // TODO: missing copy region parameters
    cmd_buffer.copyImage(src.handle(), dst.handle());
    return *this;
  }

  CommandList& copy_texture_to_buffer(CopySourceTexture auto& src, DestinationBuffer auto& dst) {
    cmd_buffer.copyImageToBuffer(src.handle(), dst.handle());
    return *this;
  }

  CommandList& copy_buffer_to_texture(SourceBuffer auto& src, CopyDestTexture auto& dst) {
    cmd_buffer.copyBufferToImage(src.handle(), dst.handle());
    return *this;
  }

  // --- Compute and Graphics ---

  CommandList& dispatch(math::uvec3 const vec)
    requires(list_type::value == compute or Type == graphics)
  {
    cmd_buffer.dispatch(vec.x, vec.y, vec.z);
    return *this;
  }

  // --- Graphics Only ---

  CommandList& render_pass(RenderPassDesc auto const& rp)
    requires(list_type::value == graphics)
  {
    mono::tuple::visit(
        rp.render_targets,
        [this](ColorTargetDesc auto const& t) { this->barrier(t.texture.get(), ResourceState::render_target, true); },
        [this](DepthTargetDesc auto const& t) { this->barrier(t.texture.get(), ResourceState::depth_write, true); }
    );

    flush_barriers();
    begin_pass(rp);

    for (DrawCallDesc auto const& draw_call: rp.draw_area.draw_calls) {
      bind_pipeline(draw_call.pso)
          .bind_vertex_buffer(draw_call.vertex_buffer.get())
          .bind_index_buffer(draw_call.index_buffer.get())
          .set_viewport(rp.draw_area.viewport)
          .set_scissor(rp.draw_area.scissor)
          .primitive_topology(PrimitiveTopology::triangle_list)
          .draw_indexed(draw_call.draw_params);
    }

    end_pass();

    mono::tuple::for_each(rp.render_targets, [this](RenderTargetDesc auto const& target) {
      this->barrier(target.texture.get(), target.final_state);
    });

    flush_barriers();
    return *this;
  }

  CommandList& begin_pass(RenderPassDesc auto const& rp)
    requires(list_type::value == graphics)
  {
    mono::static_vector<detail::core::RenderingAttachmentInfo, 8> color_attachments {};

    mono::tuple::for_each(rp.render_targets, [&](ColorTargetDesc auto const& t) {
      color_attachments.push_back(detail::to_color_attachment(t));
    });

    detail::core::RenderingInfo rendering_info = {
      .renderArea           = detail::to_native(rp.draw_area.viewport.rect),
      .layerCount           = 1,
      .colorAttachmentCount = static_cast<std::uint32_t>(color_attachments.size()),
      .pColorAttachments    = color_attachments.data(),
    };

    cmd_buffer.beginRendering(rendering_info);
    return *this;
  }

  CommandList& end_pass()
    requires(list_type::value == graphics)
  {
    cmd_buffer.endRendering();
    return *this;
  }

  CommandList& set_viewport(Viewport const& viewport)
    requires(list_type::value == graphics)
  {
    cmd_buffer.setViewport(0, detail::to_native(viewport));
    return *this;
  }

  CommandList& set_scissor(Rect2D const& scissor)
    requires(list_type::value == graphics)
  {
    cmd_buffer.setScissor(0, detail::to_native(scissor));
    return *this;
  }

  CommandList& primitive_topology(PrimitiveTopology const topology)
    requires(list_type::value == graphics)
  {
    cmd_buffer.setPrimitiveTopology(static_cast<detail::core::PrimitiveTopology>(topology));
    return *this;
  }

  CommandList& bind_vertex_buffer(VertexBuffer auto const& vb)
    requires(list_type::value == graphics)
  {
    cmd_buffer.bindVertexBuffers(0, {vb.handle()}, {0});
    return *this;
  }

  CommandList& bind_index_buffer(IndexBuffer auto const& ib)
    requires(list_type::value == graphics)
  {
    cmd_buffer.bindIndexBuffer(ib.handle(), 0, detail::core::IndexType::eUint16);
    return *this;
  }

  CommandList& draw(DrawParameters const& params)
    requires(list_type::value == graphics)
  {
    cmd_buffer.draw(params.vertex_count, params.instance_count, params.first_vertex, params.first_instance);
    return *this;
  }

  CommandList& draw_indexed(DrawParameters const& params)
    requires(list_type::value == graphics)
  {
    cmd_buffer.drawIndexed(
        params.index_count, //
        params.instance_count, //
        params.first_index, //
        static_cast<std::int32_t>(params.first_vertex), //
        params.first_instance //
    );

    return *this;
  }


  [[nodiscard]] auto const& handle() const { return cmd_buffer; }

private:
  std::vector<detail::core::ImageMemoryBarrier2> pending_image_barriers;
  std::vector<detail::core::BufferMemoryBarrier2> pending_buffer_barriers;
  native_type cmd_buffer;
};

// --- Type aliases ---

using GraphicsCommandList = CommandList<>;

using ComputeCommandList = CommandList<compute>;

using CopyCommandList = CommandList<copy>;

} // namespace rf3d::vk
