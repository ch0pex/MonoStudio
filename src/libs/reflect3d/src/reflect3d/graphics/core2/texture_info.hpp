#pragma once

#include "reflect3d/window/utils/resolution.hpp"

#include <cstdint>
#include <utility>

namespace rf3d {

enum class TextureFormat : std::uint8_t {
  unknown,

  // --- 8-bit Color (Linear) ---
  rgba8_unorm,

  // --- 8-bit Color (sRGB) ---
  rgba8_srgb, // Albedo / Base Color textures
  bgra8_srgb, // Swapchain alternative

  // --- High Precision / G-Buffer ---
  rgb10a2_unorm, // World Normals
  rgba16_sfloat, // HDR Lighting / Emissive / Post-processing

  // --- Data / Masks (Memory Optimization) ---
  r8_unorm, // Single channel: Roughness, Metallic, AO
  rg8_unorm, // Two channels: Optimized Motion Vectors or UVs
  rg16_sfloat, // High precision two channels

  // --- Depth & Stencil ---
  d32_float,
  d24_unorm_s8_uint // Required if you need Stencil masks
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
  TextureFormat format       = TextureFormat::rgba8_unorm;
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
