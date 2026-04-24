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

class FrameIndex2 {
public:
  using counter_type = std::atomic<std::uint64_t>;

  FrameIndex2() = default;

  FrameIndex2& operator++() {
    counter.fetch_add(1, std::memory_order_release);
    return *this;
  }

  counter_type value() const { return counter.load(std::memory_order_acquire) % defaults::max_frames_in_flight; }

  counter_type operator*() const { return value(); }

  operator counter_type::value_type() const { return value(); }

private:
  counter_type counter;
};

} // namespace rf3d
