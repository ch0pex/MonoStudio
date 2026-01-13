/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file slang_context.hpp
 * @version 1.0
 * @date 13/01/2026
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include "reflect3d/render/shader/targets.hpp"

#include <assets_path.hpp>

namespace rf3d::gfx::shader {

namespace detail {

template<ShaderTarget Target>
Slang::ComPtr<slang::ISession> create_session() {
  std::array search_paths = {mono::assets_path.data()};

  auto options                         = Target::options;
  slang::SessionDesc const sessionDesc = {
    .targets                  = std::addressof(Target::description),
    .targetCount              = 1,
    .searchPaths              = search_paths.data(),
    .searchPathCount          = 1,
    .compilerOptionEntries    = options.data(),
    .compilerOptionEntryCount = options.size(),
  };

  Slang::ComPtr<slang::ISession> session;
  GlobalSession::instance()->createSession(sessionDesc, session.writeRef());

  return session;
}

} // namespace detail

template<ShaderTarget Target>
class Session {
public:
  static auto& instance() {
    static auto session = detail::create_session<Target>();
    return session;
  }

private:
  Session()                          = default;
  Session(Session const&)            = delete;
  Session(Session&&)                 = delete;
  Session& operator=(Session const&) = delete;
  Session& operator=(Session&&)      = delete;
};

} // namespace rf3d::gfx::shader
