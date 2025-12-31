#pragma once

#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance_profiles.hpp"
#include "reflect3d/graphics/vk/vk_physical_device_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"
#include "reflect3d/window/window_types.hpp"


namespace rf3d::hri::vk {

class Instance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using underlying_type = std::conditional_t<enable_validation_layers, detail::DebugInstance, detail::ReleaseInstance>;
  using context_type    = raii::Context;
  using surface_type    = raii::SurfaceKHR;
  using gpu_type        = Gpu;

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

  [[nodiscard]] gpu_type pick_gpu(BestGpuCriteria const criteria) const {
    LOG_INFO("Choosing best available GPU");
    return {
      GpuDevices {detail::pick_best_physical_device(instance.handle)},
      mono::PassKey<Instance> {},
    };
  }

  [[nodiscard]] gpu_type pick_gpu(CompatibleGpuCriteria const criteria) const {
    LOG_WARNING("CompatibleGpuCriteria not implemented yet, falling back to BestGpuCriteria");
    return pick_gpu(best_gpu_criteria);
  }

private:
  context_type context;
  underlying_type instance {context};
};

} // namespace rf3d::hri::vk
