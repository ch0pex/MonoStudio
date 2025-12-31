#pragma once

#include <mono/misc/passkey.hpp>


#include <string>
#include <vector>
#include "reflect3d/window/window_types.hpp"

namespace rf3d::gfx {

template<typename Traits>
class Renderer {
public:
  // --- Type traits ---
  using instance_type = typename Traits::instance_type;
  using gpu_type      = typename Traits::gpu_type;
  using surface_type  = typename Traits::surface_type;

  // --- Constructors ---
  explicit Renderer(instance_type&& instance) : backend_instance(std::move(instance)), gpu(backend_instance) { }

  template<typename WindowHandle>
  void create_surface(WindowHandle const window) {
    // surfaces.emplace_back(backend_instance.create_surface(window));
  }

  void render_frame() {
    for (auto& surface: surfaces) {
      // render_on_surface(surface);
    }
  }


private:
  instance_type backend_instance;
  gpu_type gpu;

  std::vector<surface_type> surfaces;
};

} // namespace rf3d::gfx
