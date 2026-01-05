#pragma once

#include <vulkan/vulkan_core.h>
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_shader.hpp"

namespace rf3d::gfx::vk {

inline constexpr std::string vertex_entry_point_name   = "vertMain";
inline constexpr std::string fragment_entry_point_name = "fragMain";


struct FixedFunctionsConfig {
  core::Format color_attachment_format = core::Format::eB8G8R8A8Unorm;
  core::Format depth_attachment_format = core::Format::eD32Sfloat;
};

class PipelineBuilder {
public:
  /*********************
   *    Type traits    *
   *********************/
  using vertex_input_info_type   = core::PipelineVertexInputStateCreateInfo;
  using input_assembly_type      = core::PipelineInputAssemblyStateCreateInfo;
  using rasterizer_info_type     = core::PipelineRasterizationStateCreateInfo;
  using multisampling_info_type  = core::PipelineMultisampleStateCreateInfo;
  using color_blending_info_type = core::PipelineColorBlendStateCreateInfo;
  using stage_info_type          = core::PipelineShaderStageCreateInfo;
  using layout_info_type         = core::PipelineLayoutCreateInfo;
  using pipeline_layout_type     = raii::PipelineLayout;
  using pipeline_info_type       = core::GraphicsPipelineCreateInfo;
  using rendering_info_type      = core::PipelineRenderingCreateInfo;


  explicit PipelineBuilder(FixedFunctionsConfig const& config = {}) :
    color_format(config.color_attachment_format), //
    depth_format(config.depth_attachment_format) //
  { }

  PipelineBuilder& vertex_stage(Shader const& shader) {
    shader_stages.emplace_back(shader.stage(core::ShaderStageFlagBits::eVertex, vertex_entry_point_name));
    return *this;
  }

  PipelineBuilder& fragment_stage(Shader const& shader) {
    shader_stages.emplace_back(shader.stage(core::ShaderStageFlagBits::eFragment, fragment_entry_point_name));
    return *this;
  }

  PipelineBuilder& tessellation_stage(Shader const& shader [[maybe_unused]]) { return *this; } // TODO

  PipelineBuilder& geometry_stage(Shader const& shader [[maybe_unused]]) { return *this; } // TODO

  PipelineBuilder& set_vertex_input(vertex_input_info_type const& info) {
    vertex_input_info = info;
    return *this;
  }

  Pipeline build(raii::Device const& device) {
    core::PipelineViewportStateCreateInfo viewport_state {.viewportCount = 1, .scissorCount = 1};

    std::vector<core::DynamicState> dynamic_states_list = {
      core::DynamicState::eViewport, //
      core::DynamicState::eScissor, //
      core::DynamicState::eCullMode, //
      core::DynamicState::eFrontFace, //
      core::DynamicState::ePrimitiveTopology, //
      core::DynamicState::eLineWidth //
    };

    core::PipelineDynamicStateCreateInfo dynamic_state_info {
      .dynamicStateCount = static_cast<std::uint32_t>(dynamic_states_list.size()),
      .pDynamicStates    = dynamic_states_list.data(),
    };

    raii::PipelineLayout pipeline_layout {device, layout_info};

    core::StructureChain<pipeline_info_type, rendering_info_type> pipeline_info_chain {
      {
        .stageCount          = static_cast<std::uint32_t>(shader_stages.size()),
        .pStages             = shader_stages.data(),
        .pVertexInputState   = &vertex_input_info,
        .pInputAssemblyState = &input_assembly,
        .pViewportState      = &viewport_state,
        .pRasterizationState = &rasterizer,
        .pMultisampleState   = &multisampling,
        .pDepthStencilState  = nullptr,
        .pColorBlendState    = &color_blending,
        .pDynamicState       = &dynamic_state_info,
        .layout              = *pipeline_layout,
      },
      {
        .colorAttachmentCount    = 1,
        .pColorAttachmentFormats = &color_format,
        // .depthAttachmentFormat   = depth_format,
      }
    };

    Pipeline pipeline {
      std::move(pipeline_layout), //
      {device, nullptr, pipeline_info_chain.get<pipeline_info_type>()} //
    };

    *this = PipelineBuilder {}; // reset builder state

    return pipeline;
  }

private:
  std::vector<stage_info_type> shader_stages;
  vertex_input_info_type vertex_input_info = defaults::vertex_input_info;
  input_assembly_type input_assembly       = defaults::input_assembly;
  rasterizer_info_type rasterizer          = defaults::rasterizer;
  multisampling_info_type multisampling    = defaults::multisampling;
  color_blending_info_type color_blending  = defaults::color_blending;
  layout_info_type layout_info             = defaults::pipeline_layout_info;
  core::Format color_format                = core::Format::eB8G8R8A8Unorm;
  core::Format depth_format                = core::Format::eD32Sfloat;
};

} // namespace rf3d::gfx::vk
