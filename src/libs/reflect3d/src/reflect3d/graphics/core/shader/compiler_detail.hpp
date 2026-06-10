/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file compiler_detail.hpp
 * @date 10/06/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/shader/target_session.hpp"
#include "reflect3d/graphics/core/shader/targets.hpp"

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


namespace rf3d::shader::detail {

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

} // namespace rf3d::shader::detail
