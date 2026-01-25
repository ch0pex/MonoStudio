#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/render/shader/compiler.hpp"

#include <mono/misc/start_lifetime_as.hpp>

#include <filesystem>
#include <ranges>


namespace rf3d::gfx::vk {

namespace detail {

core::ShaderModuleCreateInfo create_shader_module_info(std::ranges::contiguous_range auto const& bytecode) {
  auto const bytecode_size = bytecode.size() / sizeof(std::uint32_t);
  core::ShaderModuleCreateInfo const create_info {
    .codeSize = bytecode.size(),
    .pCode    = mono::start_lifetime_as_array<std::uint32_t const>(bytecode.data(), bytecode_size),
  };

  return create_info;
}

} // namespace detail

class Shader {
public:
  using byte_code_type = std::span<char const>;
  using module_type    = raii::ShaderModule;
  using device_type    = raii::Device;
  using stage_type     = core::PipelineShaderStageCreateInfo;

  explicit Shader(byte_code_type const bytecode) :
    code(bytecode), module(gpu::make_shader_module(detail::create_shader_module_info(code))) { }

  [[nodiscard]] stage_type stage(core::ShaderStageFlagBits const stage, std::string const& entry_point) const {
    return stage_type {
      .stage  = stage,
      .module = *module,
      .pName  = entry_point.c_str(),
    };
  }

private:
  byte_code_type code;
  module_type module;
};

inline Shader::byte_code_type load_shader_bytecode(std::filesystem::path const& file_path) {
  auto const bytecode = shader::compile_module<shader::SpirV>(file_path);

  if (bytecode.empty()) {
    throw std::runtime_error("Failed to open shader file: " + file_path.string());
  }

  return bytecode;
}


} // namespace rf3d::gfx::vk
