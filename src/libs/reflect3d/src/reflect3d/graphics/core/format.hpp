#pragma once

#include <cstdint>

namespace rf3d {

enum class Format : std::uint8_t {
  unknown,

  // --- 8-bit Color (Linear) ---
  r8_unorm,
  rg8_unorm,
  rgb8_unorm,
  rgba8_unorm,
  bgr8_unorm,

  // --- 8-bit Color (sRGB) ---
  rgb8_srgb,
  rgba8_srgb, // Albedo / Base Color textures
  bgra8_srgb, // Swapchain alternative

  // --- 8-bit Signed/Integer ---
  r8_snorm,
  r8_uint,
  r8_sint,
  rg8_snorm,
  rg8_uint,
  rg8_sint,

  // --- 16-bit Color (Linear) ---
  r16_unorm,
  r16_snorm,
  r16_uint,
  r16_sint,
  r16_sfloat,
  rg16_unorm,
  rg16_snorm,
  rg16_uint,
  rg16_sint,
  rg16_sfloat,
  rgb16_sfloat,
  rgba16_unorm,
  rgba16_snorm,
  rgba16_uint,
  rgba16_sint,
  rgba16_sfloat,

  // --- 32-bit Float ---
  r32_sfloat,
  rg32_sfloat,
  rgb32_sfloat,
  rgba32_sfloat,

  // --- High Precision / G-Buffer ---
  rgb10a2_unorm, // World Normals
  r11f_g11f_b10f, // Shared exponent HDR
  e5b9g9r9_ufloat, // Bloom / Tone mapping

  // --- Data / Masks (Memory Optimization) ---
  // Single channel: Roughness, Metallic, AO, Height maps
  // Two channels: Motion Vectors, UVs, Normal maps (tangent space)

  // --- Compressed Formats (BC/DXT) ---
  bc1_rgb_unorm,
  bc1_rgb_srgb,
  bc1_rgba_unorm,
  bc1_rgba_srgb,
  bc2_unorm,
  bc2_srgb,
  bc3_unorm,
  bc3_srgb,
  bc4_unorm,
  bc4_snorm,
  bc5_unorm,
  bc5_snorm,
  bc6h_ufloat,
  bc6h_sfloat,
  bc7_unorm,
  bc7_srgb,

  // --- Compressed Formats (ASTC) ---
  astc_4x4_unorm,
  astc_4x4_srgb,
  astc_5x5_unorm,
  astc_5x5_srgb,
  astc_6x6_unorm,
  astc_6x6_srgb,
  astc_8x8_unorm,
  astc_8x8_srgb,
  astc_10x10_unorm,
  astc_10x10_srgb,
  astc_12x12_unorm,
  astc_12x12_srgb,

  // --- Compressed Formats (ETC2) ---
  etc2_r8g8b8_unorm,
  etc2_r8g8b8_srgb,
  etc2_r8g8b8a1_unorm,
  etc2_r8g8b8a1_srgb,
  etc2_r8g8b8a8_unorm,
  etc2_r8g8b8a8_srgb,
  eac_r11_unorm,
  eac_r11_snorm,
  eac_r11g11_unorm,
  eac_r11g11_snorm,

  // --- Depth & Stencil ---
  d16_unorm,
  d32_float,
  d24_unorm_s8_uint,
  d32_float_s8_uint,
  s8_uint
};

} // namespace rf3d
