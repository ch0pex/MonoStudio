#pragma once

#include <GLFW/glfw3.h>
#include <mono/logging/logger.hpp>

#include <ranges>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

namespace rf3d::hri::vk {

inline std::vector<VkExtensionProperties> get_supported_extensions() {
  std::uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

  for (auto const& ext: extensions) {
    LOG_INFO("Vulkan Instance Extension: {} (version {})", ext.extensionName, ext.specVersion);
  }

  return extensions;
}

inline std::vector<char const*> get_required_extensions() {
  std::uint32_t glfw_extension_count = 0;

  auto* glfw_extensions_ptr = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::span glfw_extensions(glfw_extensions_ptr, glfw_extension_count);

  return glfw_extensions | std::ranges::to<std::vector>();
}

inline bool verify_extensions_support(
    std::ranges::range auto&& supported_extensions, std::ranges::range auto&& required_extensions
) {
  auto supported_names = supported_extensions | std::views::transform([](auto const& ext) { //
                           return std::string {ext.extensionName};
                         });

  return std::ranges::all_of(required_extensions, [&](auto const* required_ext) {
    bool found = std::ranges::any_of(supported_names, [required_ext](std::string const& name) {
      return std::strcmp(required_ext, name.c_str()) == 0;
    });

    if (!found) {
      LOG_ERROR("Required Vulkan extension not supported: {}", required_ext);
    }
    return found;
  });
}

inline void ensure_extensions_support() {
  if (verify_extensions_support(get_supported_extensions(), get_required_extensions())) {
    LOG_INFO("All required Vulkan extensions are supported.");
  }
  throw std::runtime_error("Not all required Vulkan extensions are supported.");
}

} // namespace rf3d::hri::vk
