#pragma once

#include "mono/error/expected.hpp"
#include "mono/logging/logger.hpp"
#include "reflect3d/graphics/instance_config.hpp"
#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_extensions.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <GLFW/glfw3.h>
#include <expected>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

inline VkApplicationInfo create_instance_info(std::string_view const name, std::string_view const engine_name) {
  VkApplicationInfo app_info {};
  app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = name.data();
  app_info.applicationVersion = VK_MAKE_VERSION(1, 4, 0);
  app_info.pEngineName        = engine_name.data();
  app_info.engineVersion      = VK_MAKE_VERSION(1, 4, 0);
  app_info.apiVersion         = VK_API_VERSION_1_4;
  return app_info;
}

/**
 * Creates a native Vulkan instance based on the provided configuration.
 *
 * @return A VkInstance handle representing the created Vulkan instance.
 */
VkInstance create_instance(InstanceConfig const& config) {
  VkInstanceCreateInfo create_info {};
  VkInstance instance {};

  // ApplicationInfo creation
  VkApplicationInfo app_info   = create_instance_info(config.application_name, config.engine_name);
  create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  // Extensions

  auto const extensions               = get_extensions().value(); // Will throw if not supported
  create_info.enabledExtensionCount   = extensions.size();
  create_info.ppEnabledExtensionNames = not extensions.empty() ? extensions.data() : nullptr;

  // Validation layers
  auto const layers_fallback = [](std::runtime_error const& error) {
    LOG_WARNING("{}", error.what());
    return mono::err::expected<std::vector<char const*>> {};
  };

  auto const validation_layers    = get_validation_layers().or_else(layers_fallback).value();
  create_info.enabledLayerCount   = validation_layers.size();
  create_info.ppEnabledLayerNames = not validation_layers.empty() ? validation_layers.data() : nullptr;

  // Instance creation
  vkCreateInstance(&create_info, nullptr, &instance) >> check::error;

  return instance;
}


} // namespace rf3d::hri::vk::detail
