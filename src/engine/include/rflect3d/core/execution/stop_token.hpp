#pragma once
#include <csignal>
#include <stop_token>
#include "rflect3d/core/logging/logger.hpp"

namespace rflect::ex {

namespace detail {

/*
 * Vixel program execution stop token.
 * This source can be used to stop the program execution main loop
 */
inline std::stop_source stop_source {};

} // namespace detail


inline auto request_stop = [](int = {}) {
  LOG_INFO("Program execution stop was requested");
  detail::stop_source.request_stop();
};

[[nodiscard]] inline bool stop_requested() { return detail::stop_source.stop_requested(); }

[[nodiscard]] inline bool should_run() { return not stop_requested(); }

} // namespace rflect::ex
