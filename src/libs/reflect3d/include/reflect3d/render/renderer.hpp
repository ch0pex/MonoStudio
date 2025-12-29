#pragma once

#include <mono/misc/passkey.hpp>


#include <string>

namespace rf3d::gfx {

template<typename Traits>
class Renderer {
public:
  // --- Type traits ---
  using instance_type = typename Traits::instance_type;
  using gpu_type      = typename Traits::gpu_type;

  // --- Constructors ---
  explicit Renderer(instance_type&& instance) : backend_instance(std::move(instance)), gpu(backend_instance) { }


private:
  instance_type backend_instance;
  gpu_type gpu;
};

} // namespace rf3d::gfx
