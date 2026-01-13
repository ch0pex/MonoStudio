#pragma once

#include <slang-com-helper.h>
#include <slang-com-ptr.h>
#include <slang.h>

#include "global_session.hpp"
#include "mono/logging/logger.hpp"

namespace rf3d::gfx::shader {

struct ShaderTargetTag { };

template<typename T>
concept ShaderTarget = std::is_base_of_v<ShaderTargetTag, T> and requires {
  { T::description } -> std::same_as<slang::TargetDesc const&>;
  { T::options } -> std::same_as<std::array<slang::CompilerOptionEntry, 1> const&>;
};

struct SpirV : ShaderTargetTag {
  inline static slang::TargetDesc const description = {
    .format  = SLANG_SPIRV,
    .profile = GlobalSession::instance()->findProfile("spirv-1_5"),
    .flags   = SLANG_TARGET_FLAG_GENERATE_SPIRV_DIRECTLY,
  };

  static constexpr std::array<slang::CompilerOptionEntry, 1> options = {
    {slang::CompilerOptionName::EmitSpirvDirectly,
     {
       slang::CompilerOptionValueKind::Int,
       1,
       0,
       nullptr,
       nullptr,
     }}
  };
};

struct HLSL : ShaderTargetTag { };

struct DXIL : ShaderTargetTag { };

struct GLSL : ShaderTargetTag { };

struct Metal : ShaderTargetTag { };

struct WGSL : ShaderTargetTag { };

struct CUDA : ShaderTargetTag { };


} // namespace rf3d::gfx::shader
