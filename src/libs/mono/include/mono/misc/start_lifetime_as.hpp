#pragma once

#include <cstddef>
#include <new>

namespace mono {

/**
 * This is a replacement for std::start_lifetime_as_array from C++23
 * until it is implemented in standard libraries.
 *
 * @note this is not excactly the same as std::start_lifetime_as_array,
 * since magic compiler tricks are not used here but is safer to use than reinterpret_cast.
 */
template<typename T>
T* start_lifetime_as_array(void* p, std::size_t const n) noexcept {
  auto* const bytes = new (p) std::byte[sizeof(T) * n]; // NOLINT
  auto const ptr    = reinterpret_cast<T*>(bytes); // NOLINT
  (void)*ptr;
  return ptr;
}

template<typename T>
T const* start_lifetime_as_array(void const* p, std::size_t const n) noexcept {
  auto* const mp = const_cast<void*>(p); // NOLINT
  return start_lifetime_as_array<T>(mp, n);
}

template<typename T>
T* start_lifetime_as(void* p) noexcept {
  return start_lifetime_as_array<T>(p, 1);
}

template<typename T>
T const* start_lifetime_as(void const* p) noexcept {
  return start_lifetime_as_array<T>(p, 1);
}

} // namespace mono
