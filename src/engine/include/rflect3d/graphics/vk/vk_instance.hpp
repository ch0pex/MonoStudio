#pragma once


#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace rflect::gfx::vk {

namespace detail {

inline constexpr VkApplicationInfo app_info {
  .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
  .pNext              = nullptr,
  .pApplicationName   = "MonoGame",
  .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
  .pEngineName        = "Rflect3d",
  .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
  .apiVersion         = VK_API_VERSION_1_0,
};

VkInstanceCreateInfo instance_info() {
  return {
    .sType                   = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext                   = nullptr,
    .flags                   = {},
    .pApplicationInfo        = &app_info,
    .enabledLayerCount       = 0,
    .ppEnabledLayerNames     = nullptr,
    .enabledExtensionCount   = 0,
    .ppEnabledExtensionNames = nullptr,
  };
}

VkInstance create_instance() {
  VkInstance instance {};
  VkInstanceCreateInfo info = instance_info();
  if (vkCreateInstance(&info, nullptr, &instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create vulkan instance!");
  return instance;
}

} // namespace detail

class Instance {
public:
  Instance() : instance(detail::create_instance()) { }

private:
  VkInstance instance;
};

} // namespace rflect::gfx::vk
