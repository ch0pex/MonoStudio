/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file renderender_passass.hpp
 * @draw_areate 30/03/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---

// --- Dependencies ---
#include <mono/containers/tuple.hpp>
#include <mono/meta/concepts.hpp>

// --- External dependencies ---

// --- STD ---
#include <concepts>
#include <string_view>
#include "mono/misc/reference.hpp"

// --- System ---

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
concept DrawCallDesc = requires(T& draw_call) {
  { draw_call.pso } -> std::same_as<mono::Ref<typename T::pso_type>&>;
  { draw_call.vertex_buffer } -> std::same_as<mono::Ref<typename T::vertex_buffer_type>&>;
  { draw_call.index_buffer } -> std::same_as<mono::Ref<typename T::index_buffer_type>&>;
  { draw_call.draw_params } -> std::same_as<typename T::draw_params_type&>;
};

template<typename T>
concept DrawAreaDesc = requires(T& draw_area) {
  { draw_area.draw_calls } -> std::ranges::range;
  { draw_area.render_area } -> std::same_as<typename T::render_area_type&>;
};

template<typename T>
concept RenderPassDesc = requires(T& render_pass) {
  { render_pass.debug_name } -> std::convertible_to<std::string_view>;
  { render_pass.render_targets } -> mono::tuple::max_capacity<8 + 1>; // TODO: max color targets
  { render_pass.draw_area } -> std::same_as<typename T::draw_area_type&>;
};


} // namespace rf3d
