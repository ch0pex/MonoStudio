#pragma once

#include "reflect3d/graphics/core/mesh.hpp"
#include "reflect3d/window/window.hpp"

#include <experimental/propagate_const>
#include <mono/misc/as_span.hpp>

namespace rf3d::gfx::vk {

class Surface;

struct FrameInfo { };

class Core {
public:
  using surface_type = Surface;

  Core();

  Core(Core const&) = delete;

  Core(Core&&) = default;

  Core& operator=(Core const&) = delete;

  Core& operator=(Core&&) = default;

  ~Core();

  void render_surface(surface_type& surface, [[maybe_unused]] FrameInfo const& frame_info) const;

  void add_mesh(Mesh const& mesh);

  surface_type create_surface(Window&& window);

private:
  struct impl;

  std::experimental::propagate_const<std::unique_ptr<impl>> pimpl;
};


} // namespace rf3d::gfx::vk
