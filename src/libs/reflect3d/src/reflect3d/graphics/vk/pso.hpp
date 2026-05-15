#pragma once

#include "reflect3d/graphics/core/pso_descriptor.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_format_native.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_to_native.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"
#include "reflect3d/graphics/vk/detail/vk_shader.hpp"

#include <vector>

#include "reflect3d/graphics/core/shader/reflection.hpp"

namespace rf3d::vk {

namespace detail {

inline constexpr std::array dynamic_states_list = {
  core::DynamicState::eViewport,
  core::DynamicState::eScissor,
  core::DynamicState::ePrimitiveTopology,
};

inline constexpr core::PipelineDynamicStateCreateInfo dynamic_state_info {
  .dynamicStateCount = static_cast<std::uint32_t>(dynamic_states_list.size()),
  .pDynamicStates    = dynamic_states_list.data(),
};


/**
 * @brief Translates API agnostic vertex bindings to vulkan native bindings
 *
 * @param vertex_bindings API agnostic vertex binding descriptors
 * @return Vulkan Vertex Binding and Attributes
 */
inline auto translate_vertex_bindings(mono::span<VertexBufferBinding const> const vertex_bindings) {
  std::vector<core::VertexInputBindingDescription> binding_descriptions;
  std::vector<core::VertexInputAttributeDescription> attribute_descriptions;

  std::uint32_t location = 0;
  for (auto [binding, vb]: vertex_bindings | std::views::enumerate) {
    binding_descriptions.push_back({
      .binding   = static_cast<std::uint32_t>(binding),
      .stride    = vb.byte_stride,
      .inputRate = core::VertexInputRate::eVertex,
    });

    for (auto const& [offset, format]: vb.attributes) {
      attribute_descriptions.push_back({
        .location = location++,
        .binding  = static_cast<std::uint32_t>(binding),
        .format   = to_native(format),
        .offset   = offset,
      });
    }
  }

  return std::make_tuple(std::move(binding_descriptions), std::move(attribute_descriptions));
}

inline core::PipelineVertexInputStateCreateInfo create_vertex_input_state(
    mono::span<core::VertexInputBindingDescription const> const vertex_bindings,
    mono::span<core::VertexInputAttributeDescription const> const attributes
) {
  return {
    .vertexBindingDescriptionCount   = static_cast<std::uint32_t>(vertex_bindings.size()),
    .pVertexBindingDescriptions      = vertex_bindings.data(),
    .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributes.size()),
    .pVertexAttributeDescriptions    = attributes.data(),
  };
}

/**
 * @brief Creates a Vulkan graphics pipeline state object from an API agnostic pipeline creation descriptor.
 *
 * @param create_info /PSO description
 * @return Vulkan PSO
 */
inline raii::Pipeline create_pipeline(PipelineCreateInfo const& create_info) {
  Shader const shader {create_info.shader.bytecode()};

  std::array const shader_stages = {
    shader.stage(core::ShaderStageFlagBits::eVertex, "main"),
    shader.stage(core::ShaderStageFlagBits::eFragment, "main"),
  };

  auto const vertex_bindings_info = create_info.vertex_buffer_bindings.empty()
                                        ? shader::find_vertex_bindings(create_info.shader)
                                        : create_info.vertex_buffer_bindings;

  auto const [bindings, attributes] = translate_vertex_bindings(vertex_bindings_info);
  auto const vertex_input_info      = create_vertex_input_state(bindings, attributes);

  // --- Fixed-function state from RasterizerState ---
  auto const input_assembly = to_native_input_assembly(create_info.rasterizer_state);
  auto const rasterizer     = to_native_rasterizer(create_info.rasterizer_state);
  auto const depth_stencil  = to_native_depth_stencil(create_info.rasterizer_state);

  bool const has_depth = create_info.rasterizer_state.depth_mode != DepthMode::none;

  // --- Pipeline layout ---
  raii::PipelineLayout pipeline_layout = make_pipeline_layout(defaults::pipeline_layout_info);

  // --- Assemble pipeline ---
  core::PipelineViewportStateCreateInfo viewport_state {.viewportCount = 1, .scissorCount = 1};
  auto color_format = core::Format::eB8G8R8A8Srgb;

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

  explicit PipelineState(config_type const& config) : pso_handle(detail::create_pipeline(config)) { }

  [[nodiscard]] PsoType type() const { // NOLINT
    return PsoType::graphics; // TODO: support compute pipelines
  }

  [[nodiscard]] handle_type const& handle() const { return pso_handle; }

private:
  handle_type pso_handle;
};

} // namespace rf3d::vk
