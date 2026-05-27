/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_format_native.hpp
 * @date 26/05/2026
 * @brief Bidirectional conversion between reflect3d Format and Vulkan native format enums
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/format.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::vk::detail {

// clang-format off
constexpr core::Format to_native_format(Format const format) noexcept {
  switch (format) {
    case Format::unknown:           return core::Format::eUndefined;

    // --- 8-bit Color (Linear) ---
    case Format::r8_unorm:          return core::Format::eR8Unorm;
    case Format::rg8_unorm:         return core::Format::eR8G8Unorm;
    case Format::rgb8_unorm:        return core::Format::eR8G8B8Unorm;
    case Format::rgba8_unorm:       return core::Format::eR8G8B8A8Unorm;
    case Format::bgr8_unorm:        return core::Format::eB8G8R8Unorm;

    // --- 8-bit Color (sRGB) ---
    case Format::rgb8_srgb:         return core::Format::eR8G8B8Srgb;
    case Format::rgba8_srgb:        return core::Format::eR8G8B8A8Srgb;
    case Format::bgra8_srgb:        return core::Format::eB8G8R8A8Srgb;

    // --- 8-bit Signed/Integer ---
    case Format::r8_snorm:          return core::Format::eR8Snorm;
    case Format::r8_uint:           return core::Format::eR8Uint;
    case Format::r8_sint:           return core::Format::eR8Sint;
    case Format::rg8_snorm:         return core::Format::eR8G8Snorm;
    case Format::rg8_uint:          return core::Format::eR8G8Uint;
    case Format::rg8_sint:          return core::Format::eR8G8Sint;

    // --- 16-bit ---
    case Format::r16_unorm:         return core::Format::eR16Unorm;
    case Format::r16_snorm:         return core::Format::eR16Snorm;
    case Format::r16_uint:          return core::Format::eR16Uint;
    case Format::r16_sint:          return core::Format::eR16Sint;
    case Format::r16_sfloat:        return core::Format::eR16Sfloat;
    case Format::rg16_unorm:        return core::Format::eR16G16Unorm;
    case Format::rg16_snorm:        return core::Format::eR16G16Snorm;
    case Format::rg16_uint:         return core::Format::eR16G16Uint;
    case Format::rg16_sint:         return core::Format::eR16G16Sint;
    case Format::rg16_sfloat:       return core::Format::eR16G16Sfloat;
    case Format::rgb16_sfloat:      return core::Format::eR16G16B16Sfloat;
    case Format::rgba16_unorm:      return core::Format::eR16G16B16A16Unorm;
    case Format::rgba16_snorm:      return core::Format::eR16G16B16A16Snorm;
    case Format::rgba16_uint:       return core::Format::eR16G16B16A16Uint;
    case Format::rgba16_sint:       return core::Format::eR16G16B16A16Sint;
    case Format::rgba16_sfloat:     return core::Format::eR16G16B16A16Sfloat;

    // --- 32-bit Float ---
    case Format::r32_sfloat:        return core::Format::eR32Sfloat;
    case Format::rg32_sfloat:       return core::Format::eR32G32Sfloat;
    case Format::rgb32_sfloat:      return core::Format::eR32G32B32Sfloat;
    case Format::rgba32_sfloat:     return core::Format::eR32G32B32A32Sfloat;

    // --- 32-bit Integer ---
    case Format::r32_uint:          return core::Format::eR32Uint;
    case Format::rg32_uint:         return core::Format::eR32G32Uint;
    case Format::rgb32_uint:        return core::Format::eR32G32B32Uint;
    case Format::rgba32_uint:       return core::Format::eR32G32B32A32Uint;
    case Format::r32_sint:          return core::Format::eR32Sint;
    case Format::rg32_sint:         return core::Format::eR32G32Sint;
    case Format::rgb32_sint:        return core::Format::eR32G32B32Sint;
    case Format::rgba32_sint:       return core::Format::eR32G32B32A32Sint;

    // --- Packed ---
    case Format::rgb10a2_unorm:     return core::Format::eA2B10G10R10UnormPack32;
    case Format::r11f_g11f_b10f:    return core::Format::eB10G11R11UfloatPack32;
    case Format::e5b9g9r9_ufloat:   return core::Format::eE5B9G9R9UfloatPack32;

    // --- BC ---
    case Format::bc1_rgb_unorm:     return core::Format::eBc1RgbUnormBlock;
    case Format::bc1_rgb_srgb:      return core::Format::eBc1RgbSrgbBlock;
    case Format::bc1_rgba_unorm:    return core::Format::eBc1RgbaUnormBlock;
    case Format::bc1_rgba_srgb:     return core::Format::eBc1RgbaSrgbBlock;
    case Format::bc2_unorm:         return core::Format::eBc2UnormBlock;
    case Format::bc2_srgb:          return core::Format::eBc2SrgbBlock;
    case Format::bc3_unorm:         return core::Format::eBc3UnormBlock;
    case Format::bc3_srgb:          return core::Format::eBc3SrgbBlock;
    case Format::bc4_unorm:         return core::Format::eBc4UnormBlock;
    case Format::bc4_snorm:         return core::Format::eBc4SnormBlock;
    case Format::bc5_unorm:         return core::Format::eBc5UnormBlock;
    case Format::bc5_snorm:         return core::Format::eBc5SnormBlock;
    case Format::bc6h_ufloat:       return core::Format::eBc6HUfloatBlock;
    case Format::bc6h_sfloat:       return core::Format::eBc6HSfloatBlock;
    case Format::bc7_unorm:         return core::Format::eBc7UnormBlock;
    case Format::bc7_srgb:          return core::Format::eBc7SrgbBlock;

    // --- ASTC ---
    case Format::astc_4x4_unorm:    return core::Format::eAstc4x4UnormBlock;
    case Format::astc_4x4_srgb:     return core::Format::eAstc4x4SrgbBlock;
    case Format::astc_5x5_unorm:    return core::Format::eAstc5x5UnormBlock;
    case Format::astc_5x5_srgb:     return core::Format::eAstc5x5SrgbBlock;
    case Format::astc_6x6_unorm:    return core::Format::eAstc6x6UnormBlock;
    case Format::astc_6x6_srgb:     return core::Format::eAstc6x6SrgbBlock;
    case Format::astc_8x8_unorm:    return core::Format::eAstc8x8UnormBlock;
    case Format::astc_8x8_srgb:     return core::Format::eAstc8x8SrgbBlock;
    case Format::astc_10x10_unorm:  return core::Format::eAstc10x10UnormBlock;
    case Format::astc_10x10_srgb:   return core::Format::eAstc10x10SrgbBlock;
    case Format::astc_12x12_unorm:  return core::Format::eAstc12x12UnormBlock;
    case Format::astc_12x12_srgb:   return core::Format::eAstc12x12SrgbBlock;

    // --- ETC2/EAC ---
    case Format::etc2_r8g8b8_unorm:   return core::Format::eEtc2R8G8B8UnormBlock;
    case Format::etc2_r8g8b8_srgb:    return core::Format::eEtc2R8G8B8SrgbBlock;
    case Format::etc2_r8g8b8a1_unorm: return core::Format::eEtc2R8G8B8A1UnormBlock;
    case Format::etc2_r8g8b8a1_srgb:  return core::Format::eEtc2R8G8B8A1SrgbBlock;
    case Format::etc2_r8g8b8a8_unorm: return core::Format::eEtc2R8G8B8A8UnormBlock;
    case Format::etc2_r8g8b8a8_srgb:  return core::Format::eEtc2R8G8B8A8SrgbBlock;
    case Format::eac_r11_unorm:       return core::Format::eEacR11UnormBlock;
    case Format::eac_r11_snorm:       return core::Format::eEacR11SnormBlock;
    case Format::eac_r11g11_unorm:    return core::Format::eEacR11G11UnormBlock;
    case Format::eac_r11g11_snorm:    return core::Format::eEacR11G11SnormBlock;

    // --- Depth & Stencil ---
    case Format::d16_unorm:           return core::Format::eD16Unorm;
    case Format::d32_float:           return core::Format::eD32Sfloat;
    case Format::d24_unorm_s8_uint:   return core::Format::eD24UnormS8Uint;
    case Format::d32_float_s8_uint:   return core::Format::eD32SfloatS8Uint;
    case Format::s8_uint:             return core::Format::eS8Uint;
  }
  return core::Format::eUndefined;
}

constexpr Format from_native_format(core::Format const native_format) noexcept {
  switch (native_format) {
    case core::Format::eUndefined:                 return Format::unknown;

    // --- 8-bit Color (Linear) ---
    case core::Format::eR8Unorm:                   return Format::r8_unorm;
    case core::Format::eR8G8Unorm:                 return Format::rg8_unorm;
    case core::Format::eR8G8B8Unorm:               return Format::rgb8_unorm;
    case core::Format::eR8G8B8A8Unorm:             return Format::rgba8_unorm;
    case core::Format::eB8G8R8Unorm:               return Format::bgr8_unorm;

    // --- 8-bit Color (sRGB) ---
    case core::Format::eR8G8B8Srgb:                return Format::rgb8_srgb;
    case core::Format::eR8G8B8A8Srgb:              return Format::rgba8_srgb;
    case core::Format::eB8G8R8A8Srgb:              return Format::bgra8_srgb;

    // --- 8-bit Signed/Integer ---
    case core::Format::eR8Snorm:                   return Format::r8_snorm;
    case core::Format::eR8Uint:                    return Format::r8_uint;
    case core::Format::eR8Sint:                    return Format::r8_sint;
    case core::Format::eR8G8Snorm:                 return Format::rg8_snorm;
    case core::Format::eR8G8Uint:                  return Format::rg8_uint;
    case core::Format::eR8G8Sint:                  return Format::rg8_sint;

    // --- 16-bit ---
    case core::Format::eR16Unorm:                  return Format::r16_unorm;
    case core::Format::eR16Snorm:                  return Format::r16_snorm;
    case core::Format::eR16Uint:                   return Format::r16_uint;
    case core::Format::eR16Sint:                   return Format::r16_sint;
    case core::Format::eR16Sfloat:                 return Format::r16_sfloat;
    case core::Format::eR16G16Unorm:               return Format::rg16_unorm;
    case core::Format::eR16G16Snorm:               return Format::rg16_snorm;
    case core::Format::eR16G16Uint:                return Format::rg16_uint;
    case core::Format::eR16G16Sint:                return Format::rg16_sint;
    case core::Format::eR16G16Sfloat:              return Format::rg16_sfloat;
    case core::Format::eR16G16B16Sfloat:           return Format::rgb16_sfloat;
    case core::Format::eR16G16B16A16Unorm:         return Format::rgba16_unorm;
    case core::Format::eR16G16B16A16Snorm:         return Format::rgba16_snorm;
    case core::Format::eR16G16B16A16Uint:          return Format::rgba16_uint;
    case core::Format::eR16G16B16A16Sint:          return Format::rgba16_sint;
    case core::Format::eR16G16B16A16Sfloat:        return Format::rgba16_sfloat;

    // --- 32-bit Float ---
    case core::Format::eR32Sfloat:                 return Format::r32_sfloat;
    case core::Format::eR32G32Sfloat:              return Format::rg32_sfloat;
    case core::Format::eR32G32B32Sfloat:           return Format::rgb32_sfloat;
    case core::Format::eR32G32B32A32Sfloat:        return Format::rgba32_sfloat;

    // --- 32-bit Integer ---
    case core::Format::eR32Uint:                   return Format::r32_uint;
    case core::Format::eR32G32Uint:                return Format::rg32_uint;
    case core::Format::eR32G32B32Uint:             return Format::rgb32_uint;
    case core::Format::eR32G32B32A32Uint:          return Format::rgba32_uint;
    case core::Format::eR32Sint:                   return Format::r32_sint;
    case core::Format::eR32G32Sint:                return Format::rg32_sint;
    case core::Format::eR32G32B32Sint:             return Format::rgb32_sint;
    case core::Format::eR32G32B32A32Sint:          return Format::rgba32_sint;

    // --- Packed ---
    case core::Format::eA2B10G10R10UnormPack32:    return Format::rgb10a2_unorm;
    case core::Format::eB10G11R11UfloatPack32:     return Format::r11f_g11f_b10f;
    case core::Format::eE5B9G9R9UfloatPack32:      return Format::e5b9g9r9_ufloat;

    // --- BC ---
    case core::Format::eBc1RgbUnormBlock:          return Format::bc1_rgb_unorm;
    case core::Format::eBc1RgbSrgbBlock:           return Format::bc1_rgb_srgb;
    case core::Format::eBc1RgbaUnormBlock:         return Format::bc1_rgba_unorm;
    case core::Format::eBc1RgbaSrgbBlock:          return Format::bc1_rgba_srgb;
    case core::Format::eBc2UnormBlock:             return Format::bc2_unorm;
    case core::Format::eBc2SrgbBlock:              return Format::bc2_srgb;
    case core::Format::eBc3UnormBlock:             return Format::bc3_unorm;
    case core::Format::eBc3SrgbBlock:              return Format::bc3_srgb;
    case core::Format::eBc4UnormBlock:             return Format::bc4_unorm;
    case core::Format::eBc4SnormBlock:             return Format::bc4_snorm;
    case core::Format::eBc5UnormBlock:             return Format::bc5_unorm;
    case core::Format::eBc5SnormBlock:             return Format::bc5_snorm;
    case core::Format::eBc6HUfloatBlock:           return Format::bc6h_ufloat;
    case core::Format::eBc6HSfloatBlock:           return Format::bc6h_sfloat;
    case core::Format::eBc7UnormBlock:             return Format::bc7_unorm;
    case core::Format::eBc7SrgbBlock:              return Format::bc7_srgb;

    // --- ASTC ---
    case core::Format::eAstc4x4UnormBlock:         return Format::astc_4x4_unorm;
    case core::Format::eAstc4x4SrgbBlock:          return Format::astc_4x4_srgb;
    case core::Format::eAstc5x5UnormBlock:         return Format::astc_5x5_unorm;
    case core::Format::eAstc5x5SrgbBlock:          return Format::astc_5x5_srgb;
    case core::Format::eAstc6x6UnormBlock:         return Format::astc_6x6_unorm;
    case core::Format::eAstc6x6SrgbBlock:          return Format::astc_6x6_srgb;
    case core::Format::eAstc8x8UnormBlock:         return Format::astc_8x8_unorm;
    case core::Format::eAstc8x8SrgbBlock:          return Format::astc_8x8_srgb;
    case core::Format::eAstc10x10UnormBlock:       return Format::astc_10x10_unorm;
    case core::Format::eAstc10x10SrgbBlock:        return Format::astc_10x10_srgb;
    case core::Format::eAstc12x12UnormBlock:       return Format::astc_12x12_unorm;
    case core::Format::eAstc12x12SrgbBlock:        return Format::astc_12x12_srgb;

    // --- ETC2/EAC ---
    case core::Format::eEtc2R8G8B8UnormBlock:     return Format::etc2_r8g8b8_unorm;
    case core::Format::eEtc2R8G8B8SrgbBlock:      return Format::etc2_r8g8b8_srgb;
    case core::Format::eEtc2R8G8B8A1UnormBlock:   return Format::etc2_r8g8b8a1_unorm;
    case core::Format::eEtc2R8G8B8A1SrgbBlock:    return Format::etc2_r8g8b8a1_srgb;
    case core::Format::eEtc2R8G8B8A8UnormBlock:   return Format::etc2_r8g8b8a8_unorm;
    case core::Format::eEtc2R8G8B8A8SrgbBlock:    return Format::etc2_r8g8b8a8_srgb;
    case core::Format::eEacR11UnormBlock:          return Format::eac_r11_unorm;
    case core::Format::eEacR11SnormBlock:          return Format::eac_r11_snorm;
    case core::Format::eEacR11G11UnormBlock:       return Format::eac_r11g11_unorm;
    case core::Format::eEacR11G11SnormBlock:       return Format::eac_r11g11_snorm;

    // --- Depth & Stencil ---
    case core::Format::eD16Unorm:                  return Format::d16_unorm;
    case core::Format::eD32Sfloat:                 return Format::d32_float;
    case core::Format::eD24UnormS8Uint:            return Format::d24_unorm_s8_uint;
    case core::Format::eD32SfloatS8Uint:           return Format::d32_float_s8_uint;
    case core::Format::eS8Uint:                    return Format::s8_uint;

    default:
      break;
  }
  return Format::unknown;
}



// clang-format on


} // namespace rf3d::vk::detail
