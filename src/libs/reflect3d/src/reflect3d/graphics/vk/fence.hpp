/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file fence.hpp
 * @date 26/05/2026
 * @brief Vulkan fence synchronization primitive implementation
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/defaults.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <chrono>

// --- System ---


namespace rf3d::vk {

/**
 * Fence class encapsulates Vulkan's synchronization primitive used to coordinate
 * CPU and GPU operations.
 *
 * @note: signaled state indicates that the GPU has completed the associated work,
 * allowing the CPU to proceed.
 */
class Fence {
public:
  // --- Type Traits ---
  using handle_type = detail::raii::Fence;

  // --- Constructors ---
  Fence() : fence(detail::make_fence()) { }

  Fence(Fence const&) = delete;

  Fence(Fence&&) = default;

  Fence& operator=(Fence const&) = delete;

  Fence& operator=(Fence&&) = default;

  ~Fence() = default;

  // --- Member Functions ---
  void wait(std::chrono::nanoseconds const timeout = rf3d::defaults::wait_timeout) {
    detail::wait_fence(*fence, timeout);
  }

  void reset() { //
    assert(signaled() && "Cannot reset a fence that is not signaled");
    detail::reset_fences({*fence});
  }

  [[nodiscard]] bool signaled() const { //
    return (fence.getStatus() == detail::core::Result::eSuccess);
  }

  [[nodiscard]] handle_type const& handle() const { //
    return fence;
  }

private:
  handle_type fence;
};

} // namespace rf3d::vk
