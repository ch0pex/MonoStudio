#pragma once

#include <concepts>

namespace rf3d {

template<typename T>
concept Semaphore = requires(T& semaphore, typename T::count_type const value, typename T::timeout_type const timeout) {
  requires std::movable<T>;
  requires not std::copyable<T>;

  typename T::handle_type;
  typename T::count_type;
  typename T::timeout_type;

  { semaphore.wait(value, timeout) } -> std::same_as<void>;
  { semaphore.signal(value) } -> std::same_as<void>;
  { semaphore.counter() } -> std::same_as<typename T::count_type>;
  { semaphore.handle() } -> std::convertible_to<typename T::handle_type const&>;
};

} // namespace rf3d
