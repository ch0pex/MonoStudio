#pragma once

#include "reflect3d/window/window.hpp"

#include <concepts>

namespace rf3d::gfx {

// --------------------------------------------
// -------- Render Hardware Interface ---------
// --------------------------------------------

template<typename Renderer>
concept RenderBackend = requires(Renderer::surface_handle surface, Window&& window) {
  typename Renderer::surface_handle;
  { Renderer::add_mesh({}) } -> std::same_as<void>;
  { Renderer::create_surface(std::move(window)) } -> std::same_as<typename Renderer::surface_handle>;
  { Renderer::destroy_surface(surface) } -> std::same_as<void>;
  { Renderer::render_surface(surface, {}) } -> std::same_as<void>;
};

// TODO: Add Surface concept

template<typename rhi>
concept RenderHardwareInterface = RenderBackend<typename rhi::impl> and requires { //
  typename rhi::Surface;
};


} // namespace rf3d::gfx
