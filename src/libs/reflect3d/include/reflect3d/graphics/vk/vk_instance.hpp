#pragma once

#include "mono/misc/passkey.hpp"
#include "reflect3d/graphics/vk/utils/vk_exception.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance_types.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"
#include "reflect3d/window/window_types.hpp"

#include <vulkan/vulkan_core.h>


namespace rf3d::hri::vk {

class Instance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using underlying_type = std::conditional_t<enable_validation_layers, detail::DebugInstance, detail::ReleaseInstance>;
  using surface_type    = VkSurfaceKHR;
  using layer_properties_type = LayerProperties;

  /**************************
   *    Member functions    *
   **************************/

  [[nodiscard]] surface_type create_surface(NativeWindow const window) const {
    surface_type surface {};
    if (glfwCreateWindowSurface(handle, window, nullptr, &surface) != VK_SUCCESS) {
      throw Exception("Failed to create window surface!");
    }
    return surface;
  }

  void destroy_surface(surface_type const surface) const { vkDestroySurfaceKHR(handle, surface, nullptr); }

  [[nodiscard]] Gpu get_gpu() const { return Gpu {handle, mono::PassKey<Instance> {}}; }

private:
  underlying_type handle;
};

} // namespace rf3d::hri::vk
