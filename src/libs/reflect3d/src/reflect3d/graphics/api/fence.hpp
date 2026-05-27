/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file fence.hpp
 * @date 26/05/2026
 * @brief Fence concept for GPU synchronization primitive constraints
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <concepts>

// --- System ---


namespace rf3d {

template<typename T>
concept Fence = requires(T& fence) {
  requires std::movable<T>;
  requires not std::copyable<T>;

  { fence.wait() };
  { fence.reset() };
  { fence.signaled() } -> std::same_as<bool>;
};

} // namespace rf3d
