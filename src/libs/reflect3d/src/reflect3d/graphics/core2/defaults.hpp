#pragma once

#include <chrono>

namespace rf3d::defaults {

inline constexpr std::uint32_t max_frames_in_flight = 2U;

inline constexpr std::chrono::nanoseconds wait_timeout = std::chrono::nanoseconds {
  std::numeric_limits<std::chrono::nanoseconds::rep>::max(),
};

} // namespace rf3d::defaults
