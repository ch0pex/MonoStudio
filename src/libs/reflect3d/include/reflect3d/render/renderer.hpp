#pragma once

#include <mono/misc/passkey.hpp>


#include <vector>

namespace rf3d::gfx {


template<typename Traits>
class Renderer {
public:
  // --- Type traits ---
  using surface_type = typename Traits::surface_type;
  using hri_type     = typename Traits::hri_type;

  // --- Constructors ---
  explicit Renderer() : backend_instance(std::move(instance)), gpu(backend_instance) { }

  void create_surface() {
    // surfaces.emplace_back(backend_instance.create_surface(window));
  }


private:
  hri_type backend_instance;
};


} // namespace rf3d::gfx
