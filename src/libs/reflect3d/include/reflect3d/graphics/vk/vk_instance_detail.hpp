#pragma once

#include "reflect3d/graphics/instance_config.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

VkInstanceCreateInfo create_instance_info(InstanceConfig const& config) {
  VkApplicationInfo app_info {};

  app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = config.application_name.c_str();
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName        = config.engine_name.c_str();
  app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion         = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info {};
  create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  return create_info;
}

} // namespace rf3d::hri::vk::detail
