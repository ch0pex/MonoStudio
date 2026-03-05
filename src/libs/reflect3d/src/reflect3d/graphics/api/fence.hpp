#pragma once

#include <concepts>

namespace rf3d::gfx {

template<typename T>
concept Fence = requires(T& fence) {
  requires std::movable<T>;
  requires not std::copyable<T>;

  { fence.wait() };
  { fence.reset() };
  { fence.is_signaled() };
};

} // namespace rf3d::gfx
