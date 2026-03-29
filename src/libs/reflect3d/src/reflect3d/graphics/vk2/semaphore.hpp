#pragma once

#include "reflect3d/graphics/core2/defaults.hpp"
#include "reflect3d/graphics/vk2/detail/vk_gpu_detail.hpp"

namespace rf3d::vk {

class Semaphore {
public:
  // --- Type traits ---
  using handle_type  = detail::raii::Semaphore;
  using count_type   = std::size_t;
  using timeout_type = std::chrono::nanoseconds;

  // --- Constructors ---
  Semaphore() : semaphore(detail::make_semaphore()) { }

  Semaphore(Semaphore const&) = delete;

  Semaphore(Semaphore&&) = default;

  Semaphore& operator=(Semaphore const&) = delete;

  Semaphore& operator=(Semaphore&&) = default;

  ~Semaphore() {
    // TODO: Deferred destruction for Sempahores
    detail::wait_idle();
  }

  // --- Member functions ---
  void wait(count_type const value, timeout_type const timeout = rf3d::defaults::wait_timeout) {
    detail::wait_semaphore(*semaphore, value, timeout);
  }

  void signal(count_type const value = 1) { detail::signal_semaphore(*semaphore, value); }

  [[nodiscard]] count_type counter() const { return semaphore.getCounterValue(); }

  [[nodiscard]] handle_type const& handle() const { return semaphore; }

private:
  handle_type semaphore;
};

} // namespace rf3d::vk
