#pragma once

#include "reflect3d/window/utils/resolution.hpp"

#include <cstdint>
#include <utility>

namespace rf3d::gfx {

enum class TextureFormat : std::uint8_t {
  rgba8_unorm,
  rgba8_srgb,
  rgba16_float,
  d32_float,
  d24_unorm_s8_uint,
};

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

[[nodiscard]] constexpr TextureUsage operator|(TextureUsage lhs, TextureUsage rhs) noexcept {
  return static_cast<TextureUsage>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

[[nodiscard]] constexpr TextureUsage operator&(TextureUsage lhs, TextureUsage rhs) noexcept {
  return static_cast<TextureUsage>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

struct TextureInfo {
  Resolution resolution      = {};
  std::uint32_t depth        = 1;
  std::uint32_t mip_levels   = 1;
  std::uint32_t array_layers = 1;
  TextureFormat format       = TextureFormat::rgba8_unorm;
  TextureDimension dimension = TextureDimension::tex2d;
};

} // namespace rf3d::gfx
