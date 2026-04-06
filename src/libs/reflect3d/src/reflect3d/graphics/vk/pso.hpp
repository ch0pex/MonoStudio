#pragma once

#include "reflect3d/graphics/core/pso_descriptor.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_format_native.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_to_native.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"
#include "reflect3d/graphics/vk/detail/vk_shader.hpp"

#include <vector>

namespace rf3d::vk {

namespace detail {

inline raii::Pipeline create_pipeline(PipelineCreateInfo const& create_info) {
  // --- Shader stages ---
  Shader vertex_shader {create_info.vertex_shader.bytecode};
  Shader fragment_shader {create_info.fragment_shader.bytecode};

  std::array shader_stages = {
    vertex_shader.stage(core::ShaderStageFlagBits::eVertex, create_info.vertex_shader.entry_point),
    fragment_shader.stage(core::ShaderStageFlagBits::eFragment, create_info.fragment_shader.entry_point),
  };

  // --- Vertex input ---
  std::vector<core::VertexInputBindingDescription> binding_descriptions;
  std::vector<core::VertexInputAttributeDescription> attribute_descriptions;

  std::uint32_t location = 0;
  for (std::uint32_t binding = 0; binding < create_info.vertex_buffer_bindings.size(); ++binding) {
    auto const& vb = create_info.vertex_buffer_bindings[binding];

    binding_descriptions.push_back({
      .binding   = binding,
      .stride    = vb.byte_stride,
      .inputRate = core::VertexInputRate::eVertex,
    });

    for (auto const& attr: vb.attributes) {
      attribute_descriptions.push_back({
        .location = location++,
        .binding  = binding,
        .format   = to_native(attr.format),
        .offset   = attr.offset,
      });
    }
  }

  core::PipelineVertexInputStateCreateInfo vertex_input_info {
    .vertexBindingDescriptionCount   = static_cast<std::uint32_t>(binding_descriptions.size()),
    .pVertexBindingDescriptions      = binding_descriptions.data(),
    .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attribute_descriptions.size()),
    .pVertexAttributeDescriptions    = attribute_descriptions.data(),
  };

  // --- Fixed-function state from RasterizerState ---
  auto const input_assembly = to_native_input_assembly(create_info.rasterizer_state);
  auto const rasterizer     = to_native_rasterizer(create_info.rasterizer_state);
  auto const depth_stencil  = to_native_depth_stencil(create_info.rasterizer_state);

  bool const has_depth = create_info.rasterizer_state.depth_mode != DepthMode::none;

  // --- Dynamic state ---
  std::array dynamic_states_list = {
    core::DynamicState::eViewport,
    core::DynamicState::eScissor,
    core::DynamicState::ePrimitiveTopology,
  };

  core::PipelineDynamicStateCreateInfo dynamic_state_info {
    .dynamicStateCount = static_cast<std::uint32_t>(dynamic_states_list.size()),
    .pDynamicStates    = dynamic_states_list.data(),
  };

  // --- Pipeline layout ---
  raii::PipelineLayout pipeline_layout = make_pipeline_layout(defaults::pipeline_layout_info);

  // --- Assemble pipeline ---
  core::PipelineViewportStateCreateInfo viewport_state {.viewportCount = 1, .scissorCount = 1};

  core::Format color_format = core::Format::eB8G8R8A8Srgb;

  core::StructureChain<core::GraphicsPipelineCreateInfo, core::PipelineRenderingCreateInfo> pipeline_info_chain {
    {
      .stageCount          = static_cast<std::uint32_t>(shader_stages.size()),
      .pStages             = shader_stages.data(),
      .pVertexInputState   = std::addressof(vertex_input_info),
      .pInputAssemblyState = std::addressof(input_assembly),
      .pViewportState      = std::addressof(viewport_state),
      .pRasterizationState = std::addressof(rasterizer),
      .pMultisampleState   = std::addressof(defaults::multisampling),
      .pDepthStencilState  = has_depth ? std::addressof(depth_stencil) : nullptr,
      .pColorBlendState    = std::addressof(defaults::color_blending),
      .pDynamicState       = std::addressof(dynamic_state_info),
      .layout              = *pipeline_layout,
    },
    {
      .colorAttachmentCount    = 1,
      .pColorAttachmentFormats = std::addressof(color_format),
    }
  };
  return make_graphics_pipeline(pipeline_info_chain.get<core::GraphicsPipelineCreateInfo>());
}

} // namespace detail


class PipelineState {
public:
  using config_type = PipelineCreateInfo;
  using handle_type = detail::raii::Pipeline;

  PipelineState(config_type const& config) : pso_handle(detail::create_pipeline(config)) { }

  [[nodiscard]] PsoType type() const { // NOLINT
    return PsoType::graphics; // TODO: support compute pipelines
  }

  [[nodiscard]] handle_type const& handle() const { return pso_handle; }

private:
  handle_type pso_handle;
};

} // namespace rf3d::vk
