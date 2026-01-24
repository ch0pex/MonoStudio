#pragma once

#include <assets_path.hpp>
#include <iostream>


#include "reflect3d/render/shader/targets.hpp"
#include "reflect3d/render/shader/types.hpp"
#include "target_session.hpp"

namespace rf3d::gfx::shader {

namespace detail {

template<ShaderTarget Target>
auto load_module(std::filesystem::path shader_path) {
  using namespace slang;
  shader_path.replace_extension();
  std::string const module_path = shader_path.string();

  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> module {Session<Target>::instance()->loadModule(module_path.c_str(), diagnosticBlob.writeRef())
  };

  if (diagnosticBlob) {
    LOG_INFO("Diagnostics: {}", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
  }

  return module;
}

} // namespace detail


template<ShaderTarget Target>
Bytecode compile_module(std::filesystem::path const& shader_path) {
  using namespace slang;
  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> const module = detail::load_module<Target>(shader_path);

  if (not module) {
    LOG_ERROR("Failed to load module.");
    return {};
  }

  Slang::ComPtr<IComponentType> linked_program;
  module->link(linked_program.writeRef(), diagnosticBlob.writeRef());

  Slang::ComPtr<IBlob> code;
  linked_program->getTargetCode(0, code.writeRef(), diagnosticBlob.writeRef());

  if (diagnosticBlob) {
    LOG_INFO("Linker Diagnostics: {} ", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
    return {};
  }

  if (not code) {
    LOG_INFO("Failed to generate SPIR-V code.");
    return {};
  }

  return std::span {static_cast<char const*>(code->getBufferPointer()), code->getBufferSize()};
}

template<ShaderTarget Target, Stage ShaderStage>
Bytecode compile(std::filesystem::path const& shader_path) {
  using namespace slang;
  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> const module = detail::load_module<Target>(shader_path.string());

  if (not module) {
    LOG_ERROR("Failed to load module.");
    return {};
  }

  Slang::ComPtr<IEntryPoint> entryPoint;
  auto const entry_point_name = shader::entry_point_name(ShaderStage);
  module->findEntryPointByName(entry_point_name.c_str(), entryPoint.writeRef());

  if (!entryPoint) {
    LOG_ERROR("Could not find entry point 'computeMain'")
    return {};
  }

  IComponentType* components[] = {module, entryPoint};
  Slang::ComPtr<IComponentType> program;
  Session<Target>::instance()->createCompositeComponentType(components, 2, program.writeRef());

  Slang::ComPtr<IBlob> code;
  program->getEntryPointCode(0, 0, code.writeRef(), diagnosticBlob.writeRef());

  if (diagnosticBlob) {
    LOG_INFO("Linker Diagnostics: {} ", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
    return {};
  }

  if (not code) {
    LOG_INFO("Failed to generate SPIR-V code.");
    return {};
  }

  return std::span {static_cast<char const*>(code->getBufferPointer()), code->getBufferSize()};
}


} // namespace rf3d::gfx::shader
