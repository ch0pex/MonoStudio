/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_shader.hpp
 * @date 26/05/2026
 * @brief Vulkan shader module wrapper and bytecode loading utilities
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/shader/compiler.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"

// --- Dependencies ---
#include <mono/misc/start_lifetime_as.hpp>

// --- External dependencies ---

// --- STD ---
#include <filesystem>
#include <ranges>

// --- System ---


namespace rf3d::vk::detail {

namespace detail {

core::ShaderModuleCreateInfo create_shader_module_info(std::ranges::contiguous_range auto const& bytecode) {
  auto const bytecode_size = bytecode.size() / sizeof(std::uint32_t);
  core::ShaderModuleCreateInfo const create_info {
    .codeSize = bytecode.size(),
    .pCode    = mono::start_lifetime_as_array<std::uint32_t>(bytecode.data(), bytecode_size),
  };

  return create_info;
}

} // namespace detail

class Shader {
public:
  using byte_code_type = mono::span<char const>;
  using module_type    = raii::ShaderModule;
  using device_type    = raii::Device;
  using stage_type     = core::PipelineShaderStageCreateInfo;

  explicit Shader(byte_code_type const bytecode) :
    code(bytecode), module(make_shader_module(detail::create_shader_module_info(code))) { }

  [[nodiscard]] stage_type stage(core::ShaderStageFlagBits const stage, char const* entry_point) const {
    return stage_type {
      .stage  = stage,
      .module = *module,
      .pName  = entry_point,
    };
  }

private:
  byte_code_type code;
  module_type module;
};

inline Shader::byte_code_type load_shader_bytecode(std::filesystem::path const& file_path) {
  auto const program = shader::compile<shader::SpirV>(file_path);

  if (not program) {
    throw std::runtime_error(std::string("Failed to load shader bytecode: ") + program.error().what());
  }

  return program->bytecode();
}


} // namespace rf3d::vk::detail
