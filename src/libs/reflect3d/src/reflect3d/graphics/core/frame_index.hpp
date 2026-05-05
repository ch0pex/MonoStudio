/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file frame_index.hpp
 * @date 15/04/2026
 * @brief Frame Index counter class
 */

#pragma once

#include "reflect3d/graphics/core/defaults.hpp"

#include <atomic>

namespace rf3d {

class FrameIndex {
public:
  // --- Type traits ---

  using counter_type = std::uint64_t;

  // --- Constructors ---

  FrameIndex() = default;

  // --- Member functions  ---

  [[nodiscard]] counter_type value() const {
    return counter.load(std::memory_order_acquire) % defaults::max_frames_in_flight;
  }

  // --- Operators ---
  FrameIndex& operator++() {
    counter.fetch_add(1, std::memory_order_release);
    return *this;
  }

  counter_type operator*() const { return value(); }

  operator counter_type() const { return value(); }

private:
  std::atomic<counter_type> counter;
};

} // namespace rf3d
