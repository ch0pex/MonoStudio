/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file renderpass.hpp
 * @date 30/03/2026
 * @brief Render-pass descriptor concepts
 */

#pragma once

#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/pso.hpp"

#include <mono/containers/tuple.hpp>
#include <mono/meta/concepts.hpp>
#include <mono/misc/reference.hpp>

#include <concepts>
#include <ranges>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rf3d {

template<typename T>
concept ColorTargetDesc = requires(T& color_target) {
  { color_target.texture } -> std::same_as<mono::Ref<typename T::texture_type>&>;
  { color_target.clear_color } -> std::same_as<typename T::clear_color_type&>;
  { color_target.format } -> std::same_as<typename T::format_type&>;
  { color_target.load_op } -> std::same_as<typename T::load_op_type&>;
  { color_target.store_op } -> std::same_as<typename T::store_op_type&>;
  { color_target.final_state } -> std::same_as<typename T::final_state_type&>;
};

template<typename T>
concept DepthTargetDesc = requires(T& depth_target) {
  { depth_target.texture } -> std::same_as<mono::Ref<typename T::texture_type>&>;
  { depth_target.clear_value } -> std::same_as<typename T::clear_value_type&>;
  { depth_target.format } -> std::same_as<typename T::format_type&>;
  { depth_target.load_op } -> std::same_as<typename T::load_op_type&>;
  { depth_target.store_op } -> std::same_as<typename T::store_op_type&>;
  { depth_target.final_state } -> std::same_as<typename T::final_state_type&>;
};

template<typename T>
concept RenderTargetDesc = ColorTargetDesc<T> or DepthTargetDesc<T>;

namespace detail {

template<typename Tuple, std::size_t... I>
consteval bool tuple_elements_are_render_targets(std::index_sequence<I...> /*unused*/) {
  return (RenderTargetDesc<std::tuple_element_t<I, Tuple>> and ...);
}

template<typename Tuple>
concept RenderTargetsTuple = //
    mono::meta::specialization_of<Tuple, std::tuple> //
    and (std::tuple_size_v<Tuple> > 0) //
    and mono::tuple::max_capacity<Tuple, 9> //
    and tuple_elements_are_render_targets<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>> {});

} // namespace detail

template<typename T>
concept DrawCallDesc = requires(T& draw_call) {
  requires PipelineState<typename T::pso_type>;
  requires VertexBuffer<typename T::vertex_buffer_type>;
  requires IndexBuffer<typename T::index_buffer_type>;
  { draw_call.pso } -> std::same_as<mono::Ref<typename T::pso_type>&>;
  { draw_call.vertex_buffer } -> std::same_as<mono::Ref<typename T::vertex_buffer_type>&>;
  { draw_call.index_buffer } -> std::same_as<mono::Ref<typename T::index_buffer_type>&>;
  { draw_call.draw_params } -> std::same_as<typename T::draw_params_type&>;
};

template<typename T>
concept DrawAreaDesc = requires(T& draw_area) {
  requires DrawCallDesc<std::remove_cvref_t<std::ranges::range_value_t<decltype(draw_area.draw_calls)>>>;
  { draw_area.draw_calls } -> std::ranges::range;
  { draw_area.viewport } -> std::same_as<typename T::viewport_type&>;
  { draw_area.scissor } -> std::same_as<typename T::scissor_type&>;
};

template<typename T>
concept RenderPassDesc = requires(T& render_pass) {
  requires detail::RenderTargetsTuple<typename T::render_targets_type>;
  requires DrawAreaDesc<typename T::draw_area_type>;
  { render_pass.debug_name } -> std::convertible_to<std::string_view>;
  { render_pass.render_targets } -> std::same_as<typename T::render_targets_type&>;
  { render_pass.draw_area } -> std::same_as<typename T::draw_area_type&>;
};

} // namespace rf3d
