#pragma once

#include "reflect3d/graphics/instance_config.hpp"
#include "reflect3d/graphics/vk/utils/vk_result.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

VkInstance create_instance(InstanceConfig const& config) {
  VkInstanceCreateInfo create_info {};
  VkApplicationInfo app_info {};
  std::uint32_t glfwExtensionCount = 0;
  char const** glfwExtensions      = nullptr;
  VkInstance instance {};

  app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = config.application_name.c_str();
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName        = config.engine_name.c_str();
  app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion         = VK_API_VERSION_1_0;

  create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  create_info.enabledExtensionCount   = glfwExtensionCount;
  create_info.ppEnabledExtensionNames = glfwExtensions;
  create_info.enabledLayerCount       = 0;

  vkCreateInstance(&create_info, nullptr, &instance) >> check::error;

  return instance;
}

} // namespace rf3d::hri::vk::detail
