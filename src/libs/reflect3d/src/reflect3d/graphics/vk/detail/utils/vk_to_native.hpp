#pragma once

#include "mono/logging/logger.hpp"
#include "reflect3d/graphics/api/renderpass.hpp"
#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/load_store_operation.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/viewport.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_barriers.hpp"

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


constexpr core::BufferUsageFlags to_native(BufferUsage usage) {
  if not consteval {
    LOG_WARNING(
        "Buffer usage translation from api to native might be inefficient when not used in a constexpr context."
    );
  }
  core::BufferUsageFlags flags {};
  auto has_flag = [](BufferUsage mask, BufferUsage flag) { return (mask & flag) == flag; };

  if (has_flag(usage, BufferUsage::source)) {
    flags |= core::BufferUsageFlagBits::eTransferSrc;
  }
  if (has_flag(usage, BufferUsage::destination)) {
    flags |= core::BufferUsageFlagBits::eTransferDst;
  }
  if (has_flag(usage, BufferUsage::uniform)) {
    flags |= core::BufferUsageFlagBits::eUniformBuffer;
  }
  if (has_flag(usage, BufferUsage::storage)) {
    flags |= core::BufferUsageFlagBits::eStorageBuffer;
  }
  if (has_flag(usage, BufferUsage::vertex)) {
    flags |= core::BufferUsageFlagBits::eVertexBuffer;
  }
  if (has_flag(usage, BufferUsage::index)) {
    flags |= core::BufferUsageFlagBits::eIndexBuffer;
  }

  return flags;
}

template<BufferUsage Usage>
consteval core::BufferUsageFlags to_native() {
  return to_native(Usage);
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

constexpr core::PipelineInputAssemblyStateCreateInfo to_native_input_assembly(RasterizerState const& state) noexcept {
  return {.topology = to_native(state.topology)};
}

constexpr core::PipelineRasterizationStateCreateInfo to_native_rasterizer(RasterizerState const& state) noexcept {
  return {
    .depthClampEnable        = core::False,
    .rasterizerDiscardEnable = core::False,
    .polygonMode             = to_native(state.fill_mode),
    .cullMode                = to_native(state.cull_mode),
    .frontFace               = to_native(state.front_face),
    .depthBiasEnable         = core::False,
    .lineWidth               = 1.0F,
  };
}

constexpr core::PipelineDepthStencilStateCreateInfo to_native_depth_stencil(RasterizerState const& state) noexcept {
  bool const depth_read  = state.depth_mode == DepthMode::read || state.depth_mode == DepthMode::read_write;
  bool const depth_write = state.depth_mode == DepthMode::write || state.depth_mode == DepthMode::read_write;

  return {
    .depthTestEnable       = depth_read || depth_write ? core::True : core::False,
    .depthWriteEnable      = depth_write ? core::True : core::False,
    .depthCompareOp        = depth_read ? to_native(state.depth_test) : core::CompareOp::eAlways,
    .depthBoundsTestEnable = core::False,
    .stencilTestEnable     = core::False,
  };
}

} // namespace rf3d::vk::detail
