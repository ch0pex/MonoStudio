#pragma once

#include <concepts>
#include <span>

namespace rf3d {

template<typename T>
concept Gpu =
    requires(T& gpu, typename T::frame_context_type& ctx, mono::span<typename T::surface_type* const> surfaces) {
      requires not std::movable<T>;
      requires not std::copyable<T>;
      typename T::frame_context_type;
      typename T::surface_type;
      // TODO: requires Surface<typename T::surface_type>;

      { T::wait_idle() } -> std::same_as<void>;
      { T::new_frame() } -> std::same_as<typename T::frame_context_type&>;
      { T::submit_frame(ctx, surfaces) } -> std::same_as<void>;
    };

} // namespace rf3d
