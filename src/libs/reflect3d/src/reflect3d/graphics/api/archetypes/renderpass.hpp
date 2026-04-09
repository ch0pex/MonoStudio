/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file renderpass.hpp
 * @date 09/04/2026
 * @brief Render-pass descriptor archetypes
 */

#pragma once

#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/pso.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/core/format.hpp"
#include "reflect3d/graphics/core/load_store_operation.hpp"
#include "reflect3d/graphics/core/renderpass_descriptor.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/viewport.hpp"

#include <mono/containers/span.hpp>
#include <mono/misc/reference.hpp>

#include <string>
#include <tuple>

namespace rf3d::archetypes {


template<typename Pso = PipelineState, typename VB = VertexBuffer, typename IB = IndexBuffer>
struct DrawCall {
  using pso_type           = Pso;
  using vertex_buffer_type = VB;
  using index_buffer_type  = IB;
  using draw_params_type   = DrawParameters;

  mono::Ref<pso_type> pso;
  mono::Ref<vertex_buffer_type> vertex_buffer;
  mono::Ref<index_buffer_type> index_buffer;
  draw_params_type draw_params {};
};

template<typename CallType = DrawCall<>>
struct DrawArea {
  using viewport_type = Viewport;
  using scissor_type  = Rect2D;
  using call_type     = CallType;

  viewport_type viewport {};
  scissor_type scissor {};
  mono::span<call_type> draw_calls {};
};

template<typename T = DepthStencilTexture>
struct DepthTarget {
  using texture_type     = T;
  using clear_value_type = float;
  using format_type      = Format;
  using load_op_type     = LoadOperation;
  using store_op_type    = StoreOperation;
  using final_state_type = ResourceState;

  mono::Ref<texture_type> texture;
  clear_value_type clear_value = 1.0F;
  format_type format           = Format::d32_float;
  load_op_type load_op         = LoadOperation::clear;
  store_op_type store_op       = StoreOperation::store;
  final_state_type final_state = ResourceState::depth_read;
};

template<typename T = RenderTargetTexture>
struct ColorTarget {
  using texture_type     = T;
  using clear_color_type = math::vec4;
  using format_type      = Format;
  using load_op_type     = LoadOperation;
  using store_op_type    = StoreOperation;
  using final_state_type = ResourceState;

  mono::Ref<texture_type> texture;
  clear_color_type clear_color = {0.0F, 0.0F, 0.0F, 1.0F};
  format_type format           = Format::rgba8_srgb;
  load_op_type load_op         = LoadOperation::clear;
  store_op_type store_op       = StoreOperation::store;
  final_state_type final_state = ResourceState::shader_resource;
};

using RenderTargets = std::tuple<ColorTarget<>, DepthTarget<>>;

template<typename DrawAreaType = DrawArea<>, typename Targets = RenderTargets>
struct RenderPass {
  using render_targets_type = Targets;
  using draw_area_type      = DrawAreaType;

  std::string debug_name {};
  render_targets_type render_targets {};
  draw_area_type draw_area {};
};

} // namespace rf3d::archetypes
