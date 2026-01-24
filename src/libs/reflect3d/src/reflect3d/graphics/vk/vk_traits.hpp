#pragma once


#include "reflect3d/graphics/vk/vk_instance.hpp"

namespace rf3d::gfx::vk {

// class Vulkan {
// public:
//   explicit Vulkan(vk::Instance const& instance) : instance(instance) { }
//
// private:
//   vk::Instance instance;
// };

struct Traits {
  using instance_type = Instance;
  using gpu_type      = Gpu;
  using surface_type  = raii::SurfaceKHR;
};

} // namespace rf3d::gfx::vk
