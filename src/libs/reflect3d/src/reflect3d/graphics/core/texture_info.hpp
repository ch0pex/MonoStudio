/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file texture_info.hpp
 * @date 26/05/2026
 * @brief Texture dimension, usage flags and texture info descriptor types
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/format.hpp"
#include "reflect3d/window/utils/resolution.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <cstdint>
#include <utility>

// --- System ---


namespace rf3d {

enum class TextureDimension : std::uint8_t {
  tex1d,
  tex2d,
  tex3d,
  cube,
};

enum class TextureUsage : std::uint8_t {
  none             = 0,
  shader_resource  = 1 << 0,
  render_target    = 1 << 1,
  depth_stencil    = 1 << 2,
  copy_source      = 1 << 3,
  copy_dest        = 1 << 4,
  unordered_access = 1 << 5,
};

[[nodiscard]] constexpr TextureUsage operator|(TextureUsage const lhs, TextureUsage const rhs) noexcept {
  return static_cast<TextureUsage>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

[[nodiscard]] constexpr TextureUsage operator&(TextureUsage const lhs, TextureUsage const rhs) noexcept {
  return static_cast<TextureUsage>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

struct TextureInfo {
  Resolution resolution      = {};
  std::uint32_t depth        = 1;
  std::uint32_t mip_levels   = 1;
  std::uint32_t array_layers = 1;
  Format format              = Format::rgba8_unorm;
  TextureDimension dimension = TextureDimension::tex2d;
};

template<typename HandleType, typename ViewType>
struct TextureView {
  using handle_type = HandleType;
  using view_type   = ViewType;

  handle_type gpu_handle;
  view_type view;
};

} // namespace rf3d
