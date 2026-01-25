#include "reflect3d/graphics/vk/vk_instance.hpp"

#include "reflect3d/graphics/vk/instance/vk_instance_profiles.hpp"
#include "reflect3d/graphics/vk/instance/vk_validation_layers.hpp"

#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>

#include <GLFW/glfw3.h>


namespace rf3d::gfx::vk::instance {

namespace {


using context_type  = raii::Context;
using instance_type = std::conditional_t<enable_validation_layers, detail::DebugInstance, detail::ReleaseInstance>;

instance_type& instance() {
  static context_type context {};
  static instance_type instance {context};
  return instance;
}

} // namespace

[[nodiscard]] raii::SurfaceKHR create_surface(NativeWindow const window) {
  LOG_INFO("Creating Vulkan surface for the window");
  core::SurfaceKHR::NativeType surface = nullptr;

  if (glfwCreateWindowSurface(*instance().handle, window, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }

  return {instance().handle, surface};
}

raii::PhysicalDevices const& physical_devices() {
  LOG_INFO("Selecting physical devices available on the system");
  static raii::PhysicalDevices physical_devices {instance().handle};

  return physical_devices;
}

} // namespace rf3d::gfx::vk::instance
