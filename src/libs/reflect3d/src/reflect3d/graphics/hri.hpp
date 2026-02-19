#pragma once

#include "reflect3d/graphics/vk/vk_traits.hpp"

namespace rf3d::gfx {

// --------------------------------------------
// ------- Hardware Renderer Interface --------
// --------------------------------------------

template<typename T>
concept Surface = requires(T surface) { true; };

template<typename T>
concept Renderer = requires(T renderer, typename T::surface_type surface) {
  { renderer.add_mesh({}) } -> std::same_as<void>;
  { renderer.render_surface(surface, {}) } -> std::same_as<void>;
};

template<typename T>
concept Hri = Surface<typename T::surface> and Renderer<typename T::renderer>;

// ----------------------------------
// ------- Supported backends -------
// ----------------------------------

using Vulkan = vk::Traits;
using Dx12   = void;
using Metal  = void;
using Gl     = void;

static_assert(Hri<Vulkan>, "Vulkan backend does not satisfy HRI concept");

} // namespace rf3d::gfx
