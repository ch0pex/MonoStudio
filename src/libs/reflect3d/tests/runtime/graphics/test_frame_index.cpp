/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_frame_index.hpp
 * @version 1.0
 * @date 15/04/2026
 * @brief Short description
 *
 * Longer description
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "reflect3d/graphics/core/defaults.hpp"
#include "reflect3d/graphics/core/frame_index.hpp"

TEST_CASE("FrameIndex2 increments and wraps around correctly") {
  rf3d::FrameIndex2 index;

  // Initial value should be 0
  CHECK(index == 0);

  // Increment the index and check values
  for (std::uint8_t i = 1; i < rf3d::defaults::max_frames_in_flight; ++i) {
    ++index;
    CHECK(index == i);
  }

  ++index;
  CHECK(index == 0);
}

TEST_CASE("Concurrent increments of FrameIndex2 are thread-safe") {
  rf3d::FrameIndex2 index;
  {
    std::vector<std::jthread> threads;
    for (int i = 0; i < 10; ++i) {
      threads.emplace_back([&index]() {
        for (int j = 0; j < 1000; ++j) {
          ++index;
        }
      });
    }
  }
  CHECK(index == (10 * 1000) % rf3d::defaults::max_frames_in_flight);
}
