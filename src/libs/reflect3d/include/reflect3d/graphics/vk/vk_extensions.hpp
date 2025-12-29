#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <mono/error/expected.hpp>
#include <mono/logging/logger.hpp>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <ranges>
#include <vector>

namespace rf3d::hri::vk {

/**
 * Checks if all required extensions are supported.
 *
 * @return true if all required extensions are supported, false otherwise.
 */
template<std::ranges::range SupportedExtensions, std::ranges::range RequiredExtensions>
  requires(
      std::same_as<std::ranges::range_value_t<SupportedExtensions>, VkExtensionProperties> and
      std::same_as<std::ranges::range_value_t<RequiredExtensions>, std::string_view>
  )
inline bool check_extensions_support(
    SupportedExtensions const& supported_extensions, //
    RequiredExtensions const& required_extensions
) {
  auto const transform_name  = [](auto const& ext) { return std::string {&ext.extensionName[0]}; };
  auto const supported_names = supported_extensions | std::views::transform(transform_name);

  return std::ranges::all_of(required_extensions, [&](auto const& required_ext) {
    auto const match_name = [&](std::string const& name) { return required_ext == name; };
    if (not std::ranges::any_of(supported_names, match_name)) {
      LOG_ERROR("Required Vulkan extension not supported: {}", required_ext);
      return false;
    }

    return true;
  });
}

/*
 * @return A vector of supported Vulkan instance extensions.
 */
inline std::vector<VkExtensionProperties> get_supported_extensions() {
  std::uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr) >> check::error;

  std::vector<VkExtensionProperties> extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()) >> check::error;

  for (auto const& ext: extensions) {
    LOG_INFO("Supported vulkan extension: {} (version {})", ext.extensionName, ext.specVersion);
  }

  return extensions;
}

/**
 * @return A vector of required Vulkan instance extensions.
 */
inline std::vector<std::string_view> get_required_extensions() {
  std::uint32_t glfw_extension_count = 0;
  auto const* glfw_extensions_ptr    = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  auto extensions = std::span {glfw_extensions_ptr, glfw_extension_count} //
                    | std::views::transform([](auto const* ext) { return std::string_view {ext}; }) //
                    | std::ranges::to<std::vector>();

  if constexpr (enable_validation_layers) {
    LOG_INFO("Validation layers enabled, adding VK_EXT_debug_utils extension");
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

/**
 * Ensures that all required extensions by glfw are supported by vulkan,
 * otherwise, throws an exception.
 */
inline mono::err::expected<std::vector<char const*>> get_extensions() {
  auto const required_extensions = get_required_extensions();

  if (not check_extensions_support(get_supported_extensions(), required_extensions)) {
    return mono::err::unexpected("Not all required Vulkan extensions are supported.");
  }

  LOG_INFO("All required extensions are supported.");
  return required_extensions //
         | std::views::transform([](auto const& ext) { return ext.data(); }) //
         | std::ranges::to<std::vector>();
}

} // namespace rf3d::hri::vk
