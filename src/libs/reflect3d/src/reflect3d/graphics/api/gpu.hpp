#pragma once

#include <concepts>

namespace rf3d::gfx {

template<typename T>
concept Gpu = requires(T& gpu) {
  requires not std::movable<T>;
  requires not std::copyable<T>;
  typename T::frame_context_type;

  { T::wait_idle() } -> std::same_as<void>;
  { T::new_frame() } -> std::same_as<typename T::frame_context_type&>;
  { T::submit_frame() } -> std::same_as<void>;
};

} // namespace rf3d::gfx
