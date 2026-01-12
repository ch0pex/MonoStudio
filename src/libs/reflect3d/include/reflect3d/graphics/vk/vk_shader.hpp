#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

#include <cstdint>
#include <mono/misc/start_lifetime_as.hpp>

#include <filesystem>
#include <fstream>
#include <ranges>
#include <span>
#include <vector>

namespace rf3d::gfx::vk {

namespace detail {

core::ShaderModuleCreateInfo create_shader_module_create_info(std::ranges::contiguous_range auto const& bytecode) {
  auto const bytecode_size = bytecode.size() / sizeof(std::uint32_t);
  core::ShaderModuleCreateInfo create_info {
    .codeSize = bytecode.size(),
    .pCode    = mono::start_lifetime_as_array<std::uint32_t const>(bytecode.data(), bytecode_size),
  };

  return create_info;
}

} // namespace detail

class Shader {
public:
  using byte_code_type = std::vector<char>;
  using module_type    = raii::ShaderModule;
  using device_type    = raii::Device;
  using stage_type     = core::PipelineShaderStageCreateInfo;

  explicit Shader(device_type const& device, byte_code_type&& bytecode) :
    code(std::move(bytecode)), handle(device, detail::create_shader_module_create_info(code)) { }

  // [[nodiscard]] module_type const& module() const noexcept { return handle; }
  //
  // [[nodiscard]] std::span<std::byte const> bytecode() const noexcept {
  //   return std::as_bytes(std::span {code.data(), code.size()});
  // }
  [[nodiscard]] stage_type stage(core::ShaderStageFlagBits stage, std::string const& entry_point) const {
    return stage_type {
      .stage  = stage,
      .module = *handle,
      .pName  = entry_point.c_str(),
    };
  }

  [[nodiscard]] module_type const* module() const noexcept { return &handle; }

private:
  byte_code_type code;
  module_type handle;
};

inline Shader::byte_code_type load_shader_bytecode(std::filesystem::path const& file_path) {
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);

  if (not file.is_open()) {
    throw std::runtime_error("Failed to open shader file: " + file_path.string());
  }

  std::vector<char> buffer(file.tellg());

  file.seekg(0, std::ios::beg);
  file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

  return buffer;
}


} // namespace rf3d::gfx::vk
