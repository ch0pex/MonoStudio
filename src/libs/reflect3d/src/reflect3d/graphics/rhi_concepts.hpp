#pragma once

#include "reflect3d/window/window.hpp"

#include <concepts>

namespace rf3d::gfx {

// --------------------------------------------
// -------- Render Hardware Interface ---------
// --------------------------------------------

template<typename Renderer>
concept RenderHardwareInterface = requires(Window&& window) {
  typename Renderer::surface_type;
  { Renderer::add_mesh({}) } -> std::same_as<void>;
  // { Renderer::create_surface(std::move(window)) } -> std::same_as<typename Renderer::surface_type>;
  // { Renderer::render_surface(surface, {}) } -> std::same_as<void>;
};
} // namespace rf3d::gfx
