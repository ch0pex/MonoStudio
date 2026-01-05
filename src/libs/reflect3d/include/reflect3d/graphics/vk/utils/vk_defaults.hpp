#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk::defaults {

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


} // namespace rf3d::gfx::vk::defaults
