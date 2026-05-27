/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file fence.hpp
 * @date 26/05/2026
 * @brief Fence archetype for GPU synchronization primitive modeling
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::archetypes {

/**
 * Fence archetype that models a GPU synchronization primitive.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Fence {
  Fence() = default;

  Fence(Fence const&) = delete;

  Fence(Fence&&) = default;

  Fence& operator=(Fence const&) = delete;

  Fence& operator=(Fence&&) = default;

  ~Fence() = default;

  void wait();

  void reset();

  bool signaled();
};

} // namespace rf3d::archetypes
