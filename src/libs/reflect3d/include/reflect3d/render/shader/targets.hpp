#pragma once

#include "reflect3d/render/shader/global_session.hpp"

#include <slang-com-helper.h>
#include <slang-com-ptr.h>
#include <slang.h>

#include <mono/logging/logger.hpp>

namespace rf3d::gfx::shader {

struct ShaderTargetTag { };

template<typename T>
concept ShaderTarget = std::is_base_of_v<ShaderTargetTag, T> and requires {
  { T::description } -> std::same_as<slang::TargetDesc const&>;
  { T::options } -> std::ranges::range;
};

struct SpirV : ShaderTargetTag {
  inline static slang::TargetDesc const description = {
    .format  = SLANG_SPIRV,
    .profile = GlobalSession::instance()->findProfile("spirv-1_5"),
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

struct HLSL : ShaderTargetTag { };

struct DXIL : ShaderTargetTag { };

struct GLSL : ShaderTargetTag { };

struct Metal : ShaderTargetTag { };

struct WGSL : ShaderTargetTag { };

struct CUDA : ShaderTargetTag { };


} // namespace rf3d::gfx::shader
