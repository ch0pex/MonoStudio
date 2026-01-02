#pragma once

#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_gpu_queues.hpp"
#include "reflect3d/graphics/vk/vk_instance_profiles.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono/meta/concepts.hpp>

namespace rf3d::gfx::vk {

class Instance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using underlying_type = std::conditional_t<enable_validation_layers, detail::DebugInstance, detail::ReleaseInstance>;
  using context_type    = raii::Context;
  using surface_type    = raii::SurfaceKHR;

  /**************************
   *    Member functions    *
   **************************/

  // TODO check surface support
  [[nodiscard]] surface_type create_surface(NativeWindow const window) const {
    core::SurfaceKHR::NativeType surface = nullptr;

    if (glfwCreateWindowSurface(*instance.handle, window, nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create window surface");
    }

    return surface_type {instance.handle, surface};
  }

  template<typename GpuType = GraphicsGpu>
    requires mono::meta::specialization_of<GpuType, Gpu>
  [[nodiscard]] GpuType create_gpu() const {
    return {detail::pick_best_physical_device(instance.handle), mono::PassKey<Instance> {}};
  }

private:
  context_type context;
  underlying_type instance {context};
};

} // namespace rf3d::gfx::vk
