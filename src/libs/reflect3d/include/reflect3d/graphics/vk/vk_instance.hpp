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

  // [[nodiscard]] void create_surface(NativeWindow const window) const { }

  [[nodiscard]] gpu_type pick_gpu(BestGpuCriteria const criteria) const {
    auto physical_device = detail::pick_best_physical_device(instance.handle);

    auto const indices  = detail::find_queue_families(physical_device);
    auto logical_device = detail::create_logical_device(context, physical_device, indices);

    LOG_INFO("Choosing best available GPU");
    return {std::move(physical_device), std::move(logical_device)};
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
