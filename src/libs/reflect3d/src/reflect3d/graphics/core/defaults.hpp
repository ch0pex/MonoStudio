#pragma once

#include <chrono>

namespace rf3d::defaults {

inline constexpr std::uint32_t max_frames_in_flight = 2U;

inline constexpr std::chrono::nanoseconds wait_timeout = std::chrono::milliseconds {5000};

} // namespace rf3d::defaults
