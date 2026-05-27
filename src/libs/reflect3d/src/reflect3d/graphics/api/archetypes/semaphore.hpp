/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file semaphore.hpp
 * @date 26/05/2026
 * @brief Semaphore archetype for GPU synchronization primitive modeling
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <chrono>
#include <cstddef>

// --- System ---


namespace rf3d::archetypes {

/**
 * Semaphore archetype that models a GPU synchronization primitive.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Semaphore {
  using handle_type  = void*; // Placeholder type for the semaphore handle
  using count_type   = std::size_t; // Placeholder type for the semaphore counter
  using timeout_type = std::chrono::nanoseconds; // Placeholder type for the semaphore wait timeout

  Semaphore() = default;

  Semaphore(Semaphore const&) = delete;

  Semaphore(Semaphore&&) = default;

  Semaphore& operator=(Semaphore const&) = delete;

  Semaphore& operator=(Semaphore&&) = default;

  ~Semaphore() = default;

  void wait(count_type value, timeout_type timeout);

  void signal(count_type value = 1);

  count_type counter();

  handle_type const& handle();
};

} // namespace rf3d::archetypes
