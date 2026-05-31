/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file compiler.hpp
 * @date 26/05/2026
 * @brief Slang shader compilation functions for single and multi entry point targets
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/shader/program.hpp"
#include "reflect3d/graphics/core/shader/target_session.hpp"
#include "reflect3d/graphics/core/shader/targets.hpp"
#include "reflect3d/graphics/core/shader/types.hpp"

// --- Dependencies ---
#include <assets_path.hpp>
#include <mono/containers/span.hpp>
#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>

// --- External dependencies ---
#include <slang-com-ptr.h>

// --- STD ---
#include <filesystem>

// --- System ---


namespace rf3d::shader {

namespace detail {

template<ShaderTarget Target>
auto load_module(std::filesystem::path shader_path) {
  using namespace slang;
  shader_path.replace_extension();
  std::string const module_path = shader_path.string();

  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> module {
    Session<Target>::instance()->loadModule(module_path.c_str(), diagnosticBlob.writeRef())
  };

  if (diagnosticBlob) {
    LOG_INFO("Diagnostics: {}", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
  }

  return module;
}

inline std::vector<Slang::ComPtr<slang::IEntryPoint>> find_entry_points(slang::IModule* module) {
  std::vector<Slang::ComPtr<slang::IEntryPoint>> entry_points;
  std::int32_t const count = module->getDefinedEntryPointCount();

  entry_points.reserve(count);
  for (std::int32_t i = 0; i < count; ++i) {
    Slang::ComPtr<slang::IEntryPoint> entry_point;
    module->getDefinedEntryPoint(i, entry_point.writeRef());
    entry_points.push_back(std::move(entry_point));
  }
  return entry_points;
}

} // namespace detail

/**
 * @brief Compiles a single pipeline stage from a module into its own bytecode blob.
 *
 * Locates the entry point whose name matches the given stage (via entry_point_name()), composes
 * it with the module, and links them. The returned Program exposes exactly one entry point through
 * its reflection layout. Works with any ShaderTarget.
 *
 * @tparam Target     The shader target format (e.g. SpirV, HLSL).
 * @param shader_path Path to the .slang source file.
 * @param stage       The pipeline stage to compile (vertex, fragment, compute, etc.).
 * @return Program for the requested stage with reflection metadata, or an error on failure.
 */
template<ShaderTarget Target>
mono::expected<Program> compile(std::filesystem::path const& shader_path, Stage stage) {
  using namespace slang;
  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> const module = detail::load_module<Target>(shader_path.string());

  if (not module) {
    return mono::unexpected("Failed to load module.");
  }

  std::string const name = entry_point_name(stage);
  Slang::ComPtr<IEntryPoint> entry_point;
  module->findEntryPointByName(name.c_str(), entry_point.writeRef());

  if (not entry_point) {
    return mono::unexpected("Couldn't find entry point '{}' in module {}.", name, shader_path.string());
  }

  std::array<IComponentType*, 2> components = {module.get(), entry_point.get()};

  Slang::ComPtr<IComponentType> program;
  Session<Target>::instance()->createCompositeComponentType(components.data(), components.size(), program.writeRef());

  Slang::ComPtr<IComponentType> linked_program;
  program->link(linked_program.writeRef(), diagnosticBlob.writeRef());

  Slang::ComPtr<IBlob> code;
  linked_program->getTargetCode(0, code.writeRef(), diagnosticBlob.writeRef());

  if (diagnosticBlob) {
    LOG_INFO("Linker Diagnostics: {}", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
  }

  if (not code) {
    return mono::unexpected("Failed to generate shader code.");
  }

  return Program {
    std::move(linked_program), Bytecode {static_cast<char const*>(code->getBufferPointer()), code->getBufferSize()}
  };
}

/**
 * @brief Compiles all entry points of a module into a single bytecode blob with full reflection.
 *
 * Use for targets that support multiple entry points per shader (SPIR-V, Metal). All entry points
 * found in the module are composed into a single linked program, so getEntryPointCount() and
 * getEntryPointByIndex() on the returned Program are fully populated.
 *
 * @tparam Target     A MultiEntryPointTarget (e.g. SpirV, Metal).
 * @param shader_path Path to the .slang source file.
 * @return Program with all entry points and reflection metadata, or an error on failure.
 */
template<MultiEntryPointTarget Target>
mono::expected<Program> compile(std::filesystem::path const& shader_path) {
  using namespace slang;
  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> const module = detail::load_module<Target>(shader_path.string());

  if (not module) {
    return mono::unexpected("Failed to load module {}.", shader_path.string());
  }

  auto const entry_points = detail::find_entry_points(module.get());

  if (entry_points.empty()) {
    return mono::unexpected(
        "Couldn't find any entry point in the specified module shader module {}", shader_path.string()
    );
  }

  std::vector<IComponentType*> components = {module.get()};
  for (const auto& entry_point: entry_points) {
    components.push_back(entry_point.get());
  }

  Slang::ComPtr<IComponentType> program;
  Session<Target>::instance()->createCompositeComponentType(components.data(), components.size(), program.writeRef());

  Slang::ComPtr<IComponentType> linked_program;
  program->link(linked_program.writeRef(), diagnosticBlob.writeRef());

  Slang::ComPtr<IBlob> code;
  linked_program->getTargetCode(0, code.writeRef(), diagnosticBlob.writeRef());

  if (diagnosticBlob) {
    LOG_INFO("Linker Diagnostics: {}", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
  }

  if (not code) {
    return mono::unexpected("Failed to generate shader code, for {}.", shader_path.string());
  }

  return Program {
    std::move(linked_program), Bytecode {static_cast<char const*>(code->getBufferPointer()), code->getBufferSize()}
  };
}

/**
 * @brief Compiles each entry point of a module into its own bytecode blob with full reflection.
 *
 * Use for targets that only support one entry point per shader (HLSL, DXIL, GLSL, WGSL, CUDA).
 * Each entry point is composed and linked independently, so every returned Program exposes exactly
 * one entry point through getEntryPointCount() / getEntryPointByIndex().
 *
 * @tparam Target     A SingleEntryPointTarget (e.g. HLSL, DXIL, GLSL).
 * @param shader_path Path to the .slang source file.
 * @return One Program per entry point found in the module, or an error on failure.
 */
template<SingleEntryPointTarget Target>
mono::expected<std::vector<Program>> compile(std::filesystem::path const& shader_path) {
  using namespace slang;
  Slang::ComPtr<IBlob> diagnosticBlob;
  Slang::ComPtr<IModule> const module = detail::load_module<Target>(shader_path.string());

  if (not module) {
    return mono::unexpected("Failed to load module: {}", shader_path.string());
  }

  auto const entry_points = detail::find_entry_points(module.get());

  if (entry_points.empty()) {
    return mono::unexpected(
        "Couldn't find any entry point in the specified module shader module {}", shader_path.string()
    );
  }

  std::vector<Program> programs;
  programs.reserve(entry_points.size());

  for (const auto& entry_point: entry_points) {
    std::array<IComponentType*, 2> components = {module.get(), entry_point.get()};

    Slang::ComPtr<IComponentType> composite;
    Session<Target>::instance()->createCompositeComponentType(
        components.data(), components.size(), composite.writeRef()
    );

    Slang::ComPtr<IComponentType> linked;
    composite->link(linked.writeRef(), diagnosticBlob.writeRef());

    if (diagnosticBlob) {
      LOG_INFO("Linker Diagnostics: {}", static_cast<char const*>(diagnosticBlob->getBufferPointer()));
    }

    Slang::ComPtr<IBlob> code;
    linked->getTargetCode(0, code.writeRef(), diagnosticBlob.writeRef());

    if (not code) {
      return mono::unexpected("Failed to generate shader code {}", shader_path.string());
    }

    programs.emplace_back(
        std::move(linked), Bytecode {static_cast<char const*>(code->getBufferPointer()), code->getBufferSize()}
    );
  }

  return programs;
}

} // namespace rf3d::shader
