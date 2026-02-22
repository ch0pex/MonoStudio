#pragma once

#include "reflect3d/graphics/core/mesh.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/misc/as_span.hpp>

namespace rf3d::gfx::vk {

class Surface;

struct FrameInfo { };

struct Core {
  using surface_type = Surface;

  static void render_surface(surface_type& surface, [[maybe_unused]] FrameInfo const& frame_info);

  static void add_mesh(Mesh const& mesh);

  static surface_type create_surface(Window&& window);
};


} // namespace rf3d::gfx::vk
