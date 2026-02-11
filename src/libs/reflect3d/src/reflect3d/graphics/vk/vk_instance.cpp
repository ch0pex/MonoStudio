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

raii::SurfaceKHR create_surface(NativeWindow const window) {
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

Allocator create_allocator(core::PhysicalDevice const physical_device, core::Device const device) {
  VmaAllocatorCreateInfo allocator_info {};

  allocator_info.physicalDevice   = physical_device;
  allocator_info.device           = device;
  allocator_info.instance         = *instance().handle;
  allocator_info.vulkanApiVersion = VK_API_VERSION_1_3;

  VmaAllocator allocator = nullptr;
  if (core::Result const result {vmaCreateAllocator(&allocator_info, &allocator)}; result != core::Result::eSuccess) {
    throw std::runtime_error("Failed to create vulkan memory allocator");
  }

  return allocator;
}

} // namespace rf3d::gfx::vk::instance
