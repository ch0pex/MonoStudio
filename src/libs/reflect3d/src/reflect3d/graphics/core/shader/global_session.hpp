/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file global_session.hpp
 * @date 26/05/2026
 * @brief Slang global session singleton for shader compilation
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---
#include <slang-com-ptr.h>

// --- STD ---

// --- System ---


namespace rf3d::shader {

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

  GlobalSession(GlobalSession const&)            = delete;
  GlobalSession(GlobalSession&&)                 = delete;
  GlobalSession& operator=(GlobalSession const&) = delete;
  GlobalSession& operator=(GlobalSession&&)      = delete;

private:
  GlobalSession()  = default;
  ~GlobalSession() = default;
};


} // namespace rf3d::shader
