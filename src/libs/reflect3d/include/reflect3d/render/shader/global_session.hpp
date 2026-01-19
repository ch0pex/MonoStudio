#pragma once

#include <slang-com-helper.h>
#include <slang-com-ptr.h>
#include <slang.h>

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

  GlobalSession(GlobalSession const&)            = delete;
  GlobalSession(GlobalSession&&)                 = delete;
  GlobalSession& operator=(GlobalSession const&) = delete;
  GlobalSession& operator=(GlobalSession&&)      = delete;

private:
  GlobalSession()  = default;
  ~GlobalSession() = default;
};


} // namespace rf3d::gfx::shader
