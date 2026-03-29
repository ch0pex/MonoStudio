#pragma once

#include <concepts>

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
