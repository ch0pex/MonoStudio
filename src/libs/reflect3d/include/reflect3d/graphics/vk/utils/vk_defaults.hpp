#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <limits>
#include <vulkan/vulkan_structs.hpp>

namespace rf3d::gfx::vk::defaults {

inline constexpr std::size_t max_frames_in_flight = 2U;
inline constexpr std::uint64_t wait_timeout       = std::numeric_limits<std::uint64_t>::max();
inline constexpr gfx::vk::core::ClearColorValue clear_color {0.0F, 0.0F, 0.0F, 1.0F};

inline constexpr core::PipelineVertexInputStateCreateInfo vertex_input_info {};

inline constexpr core::PipelineInputAssemblyStateCreateInfo input_assembly {
  .topology = core::PrimitiveTopology::eTriangleList,
};

inline constexpr core::PipelineViewportStateCreateInfo viewport_state {
  .viewportCount = 1,
  .scissorCount  = 1,
};

inline constexpr core::PipelineRasterizationStateCreateInfo rasterizer {
  .depthClampEnable        = core::False,
  .rasterizerDiscardEnable = core::False,
  .polygonMode             = core::PolygonMode::eFill,
  .cullMode                = core::CullModeFlagBits::eBack,
  .frontFace               = core::FrontFace::eClockwise,
  .depthBiasEnable         = core::False,
  .depthBiasSlopeFactor    = 1.0F,
  .lineWidth               = 1.0F
};

inline constexpr core::PipelineMultisampleStateCreateInfo multisampling {
  .rasterizationSamples = core::SampleCountFlagBits::e1, .sampleShadingEnable = core::False
};

inline constexpr core::PipelineColorBlendAttachmentState color_blend_attachment {
  .blendEnable    = core::False,
  .colorWriteMask = core::ColorComponentFlagBits::eR | core::ColorComponentFlagBits::eG |
                    core::ColorComponentFlagBits::eB | core::ColorComponentFlagBits::eA
};

inline constexpr core::PipelineColorBlendStateCreateInfo color_blending {
  .logicOpEnable   = core::False,
  .logicOp         = core::LogicOp::eCopy,
  .attachmentCount = 1,
  .pAttachments    = &color_blend_attachment
};

inline constexpr core::PipelineLayoutCreateInfo pipeline_layout_info {
  .setLayoutCount         = 0,
  .pushConstantRangeCount = 0,
};

constexpr core::Rect2D render_area(Resolution const& resolution) {
  return core::Rect2D {
    .offset = core::Offset2D {.x = 0, .y = 0},
    .extent = core::Extent2D {
      .width  = static_cast<std::uint32_t>(resolution.width),
      .height = static_cast<std::uint32_t>(resolution.height),
    },
  };
}

constexpr core::Viewport viewport(Resolution const& resolution) {
  return {
    .x        = 0.0F,
    .y        = 0.0F,
    .width    = static_cast<float>(resolution.width),
    .height   = static_cast<float>(resolution.height),
    .minDepth = 0.0F,
    .maxDepth = 1.0F,
  };
}

inline core::RenderingInfo
rendering_info(core::Rect2D const render_area, std::span<core::RenderingAttachmentInfo const> const attachment_info) {
  return {
    .renderArea           = render_area,
    .layerCount           = 1,
    .colorAttachmentCount = static_cast<std::uint32_t>(attachment_info.size()),
    .pColorAttachments    = attachment_info.data(),
  };
}

inline core::RenderingAttachmentInfo attachament_info(Image::view_type const& image_view, core::ClearColorValue color) {
  return {
    .imageView   = image_view,
    .imageLayout = gfx::vk::core::ImageLayout::eColorAttachmentOptimal,
    .loadOp      = gfx::vk::core::AttachmentLoadOp::eClear,
    .storeOp     = gfx::vk::core::AttachmentStoreOp::eStore,
    .clearValue  = gfx::vk::core::ClearColorValue {0.0F, 0.0F, 0.0F, 1.0F},
  };
}


} // namespace rf3d::gfx::vk::defaults
