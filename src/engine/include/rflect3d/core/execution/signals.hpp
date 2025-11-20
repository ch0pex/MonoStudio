#pragma once

#include "rflect3d/core/execution/stop_token.hpp"

namespace rflect::ex {

/**
 * @brief Basic signaling setup
 *
 * Setups program signals to request a stop over a stop source and
 * returns the token of such source.
 */
inline std::stop_token setup_signals() {
  std::signal(SIGINT, request_stop);
  std::signal(SIGTERM, request_stop);

  return detail::stop_source.get_token();
}

} // namespace rflect::ex
