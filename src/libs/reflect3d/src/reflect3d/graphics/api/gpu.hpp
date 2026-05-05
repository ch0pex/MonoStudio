#pragma once


#include "surface.hpp"

#include <concepts>

namespace rf3d {

template<typename T>
concept FramenContext = requires(T& ctx) {
  requires GraphicsCommandList<typename T::command_list_type>;
  requires Fence<typename T::fence_type>;

  { ctx.command_list } -> std::same_as<typename T::command_list_type&>;
  { ctx.fence } -> std::same_as<typename T::fence_type&>;
};

template<typename T>
concept Gpu =
    requires(T& gpu, typename T::frame_context_type& ctx, mono::span<typename T::surface_type* const> surfaces) {
      requires not std::movable<T>;
      requires not std::copyable<T>;
      requires FramenContext<typename T::frame_context_type>;
      requires Surface<typename T::surface_type>;

      { T::wait_idle() } -> std::same_as<void>;
      { T::new_frame() } -> std::same_as<typename T::frame_context_type&>;
      { T::submit_frame(ctx, surfaces) } -> std::same_as<void>;
    };

} // namespace rf3d
