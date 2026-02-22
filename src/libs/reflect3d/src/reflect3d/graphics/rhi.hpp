#pragma once

#include "reflect3d/graphics/backends.hpp"
#include "reflect3d/graphics/rhi_concepts.hpp"

namespace rf3d {

/*
 * Render Hardware Interface (RHI)
 *
 * By default, the RHI is an alias to the renderer of the default backend,
 * but it can be specialized for other backends if needed.
 */
struct rhi : public gfx::DefaultBackend {
  template<gfx::RenderHardwareInterface Backend>
  using impl = Backend;
};

} // namespace rf3d
