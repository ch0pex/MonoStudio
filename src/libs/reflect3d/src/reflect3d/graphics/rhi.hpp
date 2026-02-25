#pragma once

#include "reflect3d/graphics/backends.hpp"
#include "reflect3d/graphics/concepts.hpp"
#include "reflect3d/graphics/surface.hpp"

namespace rf3d {

template<gfx::RenderBackend Backend>
struct rhi_impl : public Backend {
  rhi_impl() = delete;

  using impl = Backend;

  using Surface = gfx::Surface<Backend>;
};

/*
 * Render Hardware Interface (RHI)
 *
 * By default, the RHI is an alias to the renderer of the default backend,
 * but it can be specialized for other backends if needed.
 */
using rhi = rhi_impl<gfx::DefaultBackend>;

/*
 * Specific implementation of the RHI for the Vulkan backend.
 */
namespace impl {

using vk = rhi_impl<gfx::Vulkan>;

} // namespace impl

} // namespace rf3d
