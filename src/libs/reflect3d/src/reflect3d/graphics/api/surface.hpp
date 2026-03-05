#pragma once

#include "reflect3d/window/utils/resolution.hpp"

#include <concepts>

namespace rf3d::gfx {

template<typename T>
concept Surface = requires(T& surface, T::resolution_type const& resolution) {
  requires std::movable<T>;
  requires not std::copyable<T>;
  requires std::same_as<typename T::resolution_type, Resolution>;

  typename T::image_type;

  { surface.next_image() } -> std::same_as<typename T::image_type>;
  { surface.present() } -> std::same_as<void>;
  { surface.resize(resolution) } -> std::same_as<void>;
};

} // namespace rf3d::gfx
