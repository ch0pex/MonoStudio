#pragma once


#include <vulkan/vulkan.hpp>

#include <cstdint>
#include "rflect3d/core/logging/logger.hpp"

namespace rflect::gfx::vk {

inline std::vector<VkExtensionProperties> extensions() {
  std::vector<VkExtensionProperties> extension_properties;
  std::uint32_t extension_count {0};

  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  extension_properties.resize(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties.data());

  return extension_properties;
}

inline void log_extensions() {
  LOG_INFO("Available vulkan extensions: ");
  for (auto const& extension: extensions()) {
    LOG_INFO("\t {}", extension.extensionName);
  }
}

} // namespace rflect::gfx::vk
