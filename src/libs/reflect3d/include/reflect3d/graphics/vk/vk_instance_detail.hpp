#pragma once

#include "mono/error/expected.hpp"
#include "mono/logging/logger.hpp"
#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/vk_extensions.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

inline VkApplicationInfo create_instance_info() {
  VkApplicationInfo app_info {};
  app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = "Reflect3D Application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 4, 0);
  app_info.pEngineName        = "Reflect3D Engine";
  app_info.engineVersion      = VK_MAKE_VERSION(1, 4, 0);
  app_info.apiVersion         = VK_API_VERSION_1_4;
  return app_info;
}

/**
 * Setups validation layers and debug utils info
 */
void setup_validation_layers(VkInstanceCreateInfo& create_info) {
  if constexpr (enable_validation_layers) {
    auto constexpr layers_fallback = [](std::runtime_error const& error) {
      LOG_WARNING("{}", error.what());
      return mono::err::expected<std::vector<char const*>> {};
    };

    // This needs to survive until vkCreateInstance is invoked thats why it has static storage
    static auto const validation_layers = get_validation_layers().or_else(layers_fallback).value();
    create_info.enabledLayerCount       = validation_layers.size();
    create_info.ppEnabledLayerNames     = not validation_layers.empty() ? validation_layers.data() : nullptr;
    create_info.pNext                   = &debug_utils_create_info;
  }
}

/**
 * Creates a native Vulkan instance based on the provided configuration.
 *
 * @note may throw if mandatory extensions are not supported
 * @return A VkInstance handle representing the created Vulkan instance.
 */
VkInstance create_instance() {
  VkInstanceCreateInfo create_info {};
  VkInstance instance {};

  // ApplicationInfo creation
  VkApplicationInfo app_info   = create_instance_info();
  create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  // Extensions
  auto const extensions               = get_extensions().value(); // Will throw if not supported
  create_info.enabledExtensionCount   = extensions.size();
  create_info.ppEnabledExtensionNames = not extensions.empty() ? extensions.data() : nullptr;

  setup_validation_layers(create_info);
  vkCreateInstance(&create_info, nullptr, &instance) >> check::error;

  return instance;
}


} // namespace rf3d::hri::vk::detail
