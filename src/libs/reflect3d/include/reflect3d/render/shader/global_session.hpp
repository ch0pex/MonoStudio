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

namespace rf3d::gfx::shader {

namespace detail {

inline auto create_global_session() {
  Slang::ComPtr<slang::IGlobalSession> globalSession;
  createGlobalSession(globalSession.writeRef());
  return globalSession;
}

} // namespace detail

class GlobalSession {
public:
  static auto& instance() {
    static auto global_session = detail::create_global_session();
    return global_session;
  }

private:
  GlobalSession()                                = default;
  GlobalSession(GlobalSession const&)            = delete;
  GlobalSession(GlobalSession&&)                 = delete;
  GlobalSession& operator=(GlobalSession const&) = delete;
  GlobalSession& operator=(GlobalSession&&)      = delete;
};


} // namespace rf3d::gfx::shader
