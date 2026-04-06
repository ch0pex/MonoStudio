#pragma once

#include <glm/fwd.hpp>
#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/pso.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/pso.hpp"
#include "reflect3d/graphics/api/renderpass.hpp"
#include "reflect3d/graphics/api/texture.hpp"
#include "reflect3d/graphics/core/load_store_operation.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/texture_info.hpp"
#include "reflect3d/graphics/core/viewport.hpp"
//
#include <mono/containers/span.hpp>
#include <mono/meta/concepts.hpp>
#include <mono/misc/reference.hpp>

//
#include <string>

namespace rf3d {

struct DrawParameters {
  std::uint32_t vertex_count   = 0;
  std::uint32_t instance_count = 1;
  std::uint32_t first_vertex   = 0;
  std::uint32_t first_instance = 0;
  std::uint32_t index_count    = 0;
  std::uint32_t first_index    = 0;
};

template< //
    PipelineState Pso = archetypes::PipelineState,  //
    VertexBuffer VB = archetypes::VertexBuffer, //
    IndexBuffer IB = archetypes::IndexBuffer //
>
struct DrawCall {
  using pso_type           = Pso;
  using vertex_buffer_type = VB;
  using index_buffer_type  = IB;
  using draw_params_type   = DrawParameters;

  mono::Ref<pso_type> pso {};
  mono::Ref<vertex_buffer_type> vertex_buffer {};
  mono::Ref<index_buffer_type> index_buffer {};
  draw_params_type draw_params {};
};

template<typename CallType = DrawCall<>>
struct DrawArea {
  using viewport_type = Viewport;
  using scissor_type  = Rect2D;
  using call_type     = CallType;

  viewport_type viewport {};
  scissor_type scissor {};
  mono::span<call_type> draw_calls;
};

template<DepthStencilTexture T>
struct DepthTarget {
  using texture_type     = T;
  using clear_value_type = float;
  using format_type      = Format;
  using load_op_type     = LoadOperation;
  using store_op_type    = StoreOperation;
  using final_state_type = ResourceState;

  mono::Ref<T> texture;
  float clear_value         = 1.0F;
  Format format             = Format::d32_float;
  LoadOperation load_op     = LoadOperation::clear;
  StoreOperation store_op   = StoreOperation::store;
  ResourceState final_state = ResourceState::depth_read;
};

template<typename T, typename... Args>
DepthTarget(T&, Args&&...) -> DepthTarget<T>;

template<RenderTargetTexture T = archetypes::RenderTargetTexture>
struct ColorTarget {
  using texture_type     = T;
  using clear_color_type = math::vec4;
  using format_type      = Format;
  using load_op_type     = LoadOperation;
  using store_op_type    = StoreOperation;
  using final_state_type = ResourceState;

  mono::Ref<T> texture;
  math::vec4 clear_color    = {0.0F, 0.0F, 0.0F, 1.0F};
  Format format             = Format::rgba8_srgb;
  LoadOperation load_op     = LoadOperation::clear;
  StoreOperation store_op   = StoreOperation::store;
  ResourceState final_state = ResourceState::shader_resource;
};

template<typename T, typename... Args>
ColorTarget(T&, Args&&...) -> ColorTarget<T>;

template<typename T>
concept RenderTargetDesc = ColorTargetDesc<T> or DepthTargetDesc<T>;

template<RenderTargetDesc... Args>
  requires(sizeof...(Args) <= 9 and sizeof...(Args) > 0)
using RenderTargets = std::tuple<Args...>;

template<DrawAreaDesc DrawArea, mono::meta::specialization_of<std::tuple> Ct>
struct RenderPass {
  using render_targets_type = Ct;
  using draw_area_type      = DrawArea;

  std::string debug_name {};
  render_targets_type render_targets {};
  draw_area_type draw_area {};
};

} // namespace rf3d
