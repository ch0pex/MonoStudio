#pragma once

#include "reflect3d/graphics/core/shader/global_session.hpp"

#include <slang-com-helper.h>
#include <slang-com-ptr.h>
#include <slang.h>

#include <array>
#include <ranges>

namespace rf3d::shader {

struct ShaderTargetTag { };

template<typename T>
concept ShaderTarget = std::is_base_of_v<ShaderTargetTag, T> and requires {
  { T::description } -> std::same_as<slang::TargetDesc const&>;
  { T::options } -> std::ranges::range;
  { T::multi_entry_point } -> std::same_as<bool const&>;
};

template<typename T>
concept MultiEntryPointTarget = ShaderTarget<T> and T::multi_entry_point;

template<typename T>
concept SingleEntryPointTarget = ShaderTarget<T> and not T::multi_entry_point;

struct SpirV : ShaderTargetTag {
  static constexpr bool multi_entry_point = true;

  inline static slang::TargetDesc const description = {
    .format  = SLANG_SPIRV,
    .profile = GlobalSession::instance()->findProfile("spirv_1_5"),
  };

  static constexpr std::array options = {
    slang::CompilerOptionEntry {
      .name = slang::CompilerOptionName::EmitSpirvDirectly,
      .value =
          {
            .kind         = slang::CompilerOptionValueKind::Int,
            .intValue0    = 1,
            .intValue1    = 0,
            .stringValue0 = nullptr,
            .stringValue1 = nullptr,
          },
    },
#ifndef NDEBUG
    slang::CompilerOptionEntry {
      .name  = slang::CompilerOptionName::DebugInformation,
      .value = {
        .kind         = slang::CompilerOptionValueKind::Int,
        .intValue0    = SLANG_DEBUG_INFO_LEVEL_MAXIMAL,
        .intValue1    = 0,
        .stringValue0 = nullptr,
        .stringValue1 = nullptr,
      },
    }
#endif
  };
};

struct HLSL : ShaderTargetTag { static constexpr bool multi_entry_point = false; };

struct DXIL : ShaderTargetTag { static constexpr bool multi_entry_point = false; };

struct GLSL : ShaderTargetTag { static constexpr bool multi_entry_point = false; };

struct Metal : ShaderTargetTag { static constexpr bool multi_entry_point = true; };

struct WGSL : ShaderTargetTag { static constexpr bool multi_entry_point = false; };

struct CUDA : ShaderTargetTag { static constexpr bool multi_entry_point = false; };


} // namespace rf3d::shader
