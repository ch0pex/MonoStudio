#pragma once

#include "reflect3d/graphics/core2/pso_states.hpp"
#include "reflect3d/graphics/core2/renderpass_descriptor.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/core2/viewport.hpp"
#include "reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk2/detail/vk_barriers.hpp"
#include "reflect3d/graphics/vk2/renderpass.hpp"

namespace rf3d::vk::detail {

constexpr core::PrimitiveTopology to_native(PrimitiveTopology const topology) noexcept {
  return static_cast<core::PrimitiveTopology>(topology);
}

constexpr core::CompareOp to_native(ComparisonFunction const comparison_function) noexcept {
  return static_cast<core::CompareOp>(comparison_function);
}

constexpr core::FrontFace to_native(FrontFace const front_face) noexcept {
  return static_cast<core::FrontFace>(front_face);
}

constexpr core::CullModeFlags to_native(CullMode const cull_mode) noexcept {
  return static_cast<core::CullModeFlagBits>(cull_mode);
}

constexpr core::PolygonMode to_native(PolygonMode const polygon_mode) noexcept {
  return static_cast<core::PolygonMode>(polygon_mode);
}

constexpr core::PolygonMode to_native(FillMode const fill_mode) noexcept {
  return static_cast<core::PolygonMode>(fill_mode);
}

constexpr Blend to_native(Blend const blend) noexcept { //
  return blend;
}

constexpr BlendOperation to_native(BlendOperation const blend_operation) noexcept { //
  return blend_operation;
}

constexpr PsoType to_native(PsoType const pso_type) noexcept { //
  return pso_type;
}

constexpr core::Viewport to_native(Viewport const& viewport) noexcept {
  return core::Viewport {
    .x        = static_cast<float>(viewport.rect.offset.x),
    .y        = static_cast<float>(viewport.rect.offset.y),
    .width    = static_cast<float>(viewport.rect.extent.x),
    .height   = static_cast<float>(viewport.rect.extent.y),
    .minDepth = viewport.min_depth,
    .maxDepth = viewport.max_depth
  };
}

constexpr core::Rect2D to_native(Rect2D const& scissor) noexcept {
  return core::Rect2D {
    .offset =
        core::Offset2D {
          .x = static_cast<std::int32_t>(scissor.offset.x),
          .y = static_cast<std::int32_t>(scissor.offset.y),
        },
    .extent = core::Extent2D {
      .width  = static_cast<std::uint32_t>(scissor.extent.x),
      .height = static_cast<std::uint32_t>(scissor.extent.y),
    },
  };
}

constexpr BarrierInfo to_native(ResourceState const state) noexcept {
  using namespace core;

  switch (state) {
    case ResourceState::undefined:
      return {
        .stage_mask   = PipelineStageFlagBits2::eNone,
        .access_mask  = AccessFlagBits2::eNone,
        .image_layout = ImageLayout::eUndefined
      };

    case ResourceState::vertex_buffer:
    case ResourceState::index_buffer:
      return {
        .stage_mask   = PipelineStageFlagBits2::eVertexInput,
        .access_mask  = (state == ResourceState::vertex_buffer) ? AccessFlagBits2::eVertexAttributeRead
                                                                : AccessFlagBits2::eIndexRead,
        .image_layout = ImageLayout::eUndefined
      };

    case ResourceState::constant_buffer:
      return {
        .stage_mask   = PipelineStageFlagBits2::eAllGraphics | PipelineStageFlagBits2::eComputeShader,
        .access_mask  = AccessFlagBits2::eUniformRead,
        .image_layout = ImageLayout::eUndefined
      };

    case ResourceState::shader_resource:
      return {
        .stage_mask   = PipelineStageFlagBits2::eAllGraphics | PipelineStageFlagBits2::eComputeShader,
        .access_mask  = AccessFlagBits2::eShaderRead,
        .image_layout = ImageLayout::eShaderReadOnlyOptimal
      };

    case ResourceState::unordered_access:
      return {
        .stage_mask   = PipelineStageFlagBits2::eComputeShader | PipelineStageFlagBits2::eFragmentShader,
        .access_mask  = AccessFlagBits2::eShaderRead | AccessFlagBits2::eShaderWrite,
        .image_layout = ImageLayout::eGeneral
      };

    case ResourceState::copy_destination:
      return {
        .stage_mask   = PipelineStageFlagBits2::eTransfer,
        .access_mask  = AccessFlagBits2::eTransferWrite,
        .image_layout = ImageLayout::eTransferDstOptimal
      };

    case ResourceState::copy_source:
      return {
        .stage_mask   = PipelineStageFlagBits2::eTransfer,
        .access_mask  = AccessFlagBits2::eTransferRead,
        .image_layout = ImageLayout::eTransferSrcOptimal
      };

    case ResourceState::render_target:
      return {
        .stage_mask   = PipelineStageFlagBits2::eColorAttachmentOutput,
        .access_mask  = AccessFlagBits2::eColorAttachmentWrite | AccessFlagBits2::eColorAttachmentRead,
        .image_layout = ImageLayout::eColorAttachmentOptimal
      };

    case ResourceState::depth_write:
      return {
        .stage_mask   = PipelineStageFlagBits2::eEarlyFragmentTests | PipelineStageFlagBits2::eLateFragmentTests,
        .access_mask  = AccessFlagBits2::eDepthStencilAttachmentWrite,
        .image_layout = ImageLayout::eDepthStencilAttachmentOptimal
      };

    case ResourceState::depth_read:
      return {
        .stage_mask   = PipelineStageFlagBits2::eEarlyFragmentTests | PipelineStageFlagBits2::eLateFragmentTests,
        .access_mask  = AccessFlagBits2::eDepthStencilAttachmentRead,
        .image_layout = ImageLayout::eDepthStencilReadOnlyOptimal
      };

    case ResourceState::indirect_argument:
      return {
        .stage_mask   = PipelineStageFlagBits2::eDrawIndirect,
        .access_mask  = AccessFlagBits2::eIndirectCommandRead,
        .image_layout = ImageLayout::eUndefined
      };

    case ResourceState::present:
      return {
        .stage_mask   = PipelineStageFlagBits2::eNone,
        .access_mask  = AccessFlagBits2::eNone,
        .image_layout = ImageLayout::ePresentSrcKHR
      };

    default:
      return {
        .stage_mask   = PipelineStageFlagBits2::eAllCommands,
        .access_mask  = AccessFlagBits2::eMemoryRead | AccessFlagBits2::eMemoryWrite,
        .image_layout = ImageLayout::eGeneral
      };
  }
}

constexpr core::AttachmentLoadOp to_native(LoadOperation const load_op) noexcept { //
  return static_cast<core::AttachmentLoadOp>(load_op);
}

constexpr core::AttachmentStoreOp to_native(StoreOperation const store_op) noexcept {
  return static_cast<core::AttachmentStoreOp>(store_op);
}


constexpr core::ClearColorValue to_native(math::vec4 const& clear_color) noexcept {
  return {clear_color.x, clear_color.y, clear_color.z, clear_color.w};
}

inline constexpr auto to_color_attachment = [](ColorTargetDesc auto const& target) -> core::RenderingAttachmentInfo {
  return {
    .imageView   = target.texture.get().view().view,
    .imageLayout = core::ImageLayout::eColorAttachmentOptimal,
    .loadOp      = to_native(target.load_op),
    .storeOp     = to_native(target.store_op),
    .clearValue  = to_native(target.clear_color),
  };
};

inline constexpr auto to_depth_attachment = [](DepthTargetDesc auto const& target) -> core::RenderingAttachmentInfo {
  return {
    .imageView   = target.texture.view,
    .imageLayout = core::ImageLayout::eDepthAttachmentOptimal,
    // .loadOp      = target.load_op,
    // .stopreOp    = target.store_op,
    // .clearValue = target.clear_color
  };
};

} // namespace rf3d::vk::detail
