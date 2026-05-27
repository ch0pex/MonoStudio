/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file target_session.hpp
 * @date 13/01/2026
 * @brief Slang target session singleton for shader target-specific compilation sessions
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/shader/targets.hpp"

// --- Dependencies ---
#include <assets_path.hpp>

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::shader {

namespace detail {

template<ShaderTarget Target>
Slang::ComPtr<slang::ISession> create_session() {
  std::string assets_path {mono::assets_path};
  std::array search_paths = {assets_path.c_str()};

  auto options                         = Target::options;
  slang::SessionDesc const sessionDesc = {
    .targets                  = std::addressof(Target::description),
    .targetCount              = 1,
    .searchPaths              = search_paths.data(),
    .searchPathCount          = 1,
    .compilerOptionEntries    = options.data(),
    .compilerOptionEntryCount = static_cast<std::uint32_t>(options.size()),
  };

  Slang::ComPtr<slang::ISession> session;
  GlobalSession::instance()->createSession(sessionDesc, session.writeRef());

  return session;
}

} // namespace detail

template<ShaderTarget Target>
class Session {
  Session()  = default;
  ~Session() = default;

public:
  Session(Session const&)            = delete;
  Session(Session&&)                 = delete;
  Session& operator=(Session const&) = delete;
  Session& operator=(Session&&)      = delete;
  static auto& instance() {
    static auto session = detail::create_session<Target>();
    return session;
  }
};

} // namespace rf3d::shader
