#pragma once

#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

#include <concepts>

namespace rf3d {

template<typename T>
concept Surface = requires(T& surface, T::resolution_type const& resolution, T::window_type&& window) {
  requires std::movable<T>;
  requires not std::copyable<T>;
  requires std::same_as<typename T::resolution_type, Resolution>;
  requires std::same_as<typename T::window_type, Window>;
  typename T::image_type;

  { T(std::move(window)) };
  { surface.next_image() } -> std::same_as<typename T::image_type>;
  { surface.present() } -> std::same_as<void>;
  { surface.resize(resolution) } -> std::same_as<void>;
};

} // namespace rf3d
