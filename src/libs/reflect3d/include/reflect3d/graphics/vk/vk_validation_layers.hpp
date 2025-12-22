#pragma once

#include "mono/error/expected.hpp"
#include "reflect3d/graphics/vk/utils/vk_checker.hpp"


// Mono library
#include <mono/logging/logger.hpp>

// External libraries
#include <vulkan/vulkan_core.h>

// STD library
#include <algorithm>
#include <array>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <vector>

namespace rf3d::hri::vk {

#ifdef NDEBUG
inline constexpr bool enable_validation_layers = false;
#else
inline constexpr bool enable_validation_layers = true;
#endif

using LayerProperties = VkLayerProperties;

/**
 * Given a list of supported validation layers and required validation layers,
 * returns true if all required layers are supported.
 *
 * @return true if all requested validation layers are supported.
 */
template <std::ranges::range SupportedLayers, std::ranges::range RequiredLayers>
  requires(std::same_as<std::ranges::range_value_t<SupportedLayers>, LayerProperties> and
           std::same_as<std::ranges::range_value_t<RequiredLayers>, std::string_view>)
inline bool check_validation_layer_support( //
  SupportedLayers const& supported_layers,  
  RequiredLayers const& required_layers) {

  auto const transform_name  = [](auto const& ext) { return std::string {&ext.layerName[0]}; };
  auto const supported_names = supported_layers | std::views::transform(transform_name);

  // All requested layers must be found in the supported layers
  return std::ranges::all_of(required_layers, [&](auto const& layer_name) {
    auto const match_name = [&](std::string const& name) { return layer_name == name; };
    if (not std::ranges::any_of(supported_names, match_name)) {
      LOG_ERROR("Required Vulkan validation layer not supported: {}", layer_name);
      return false;
    }

    return true;
  });
}

/**
 * @return a vector with all the supported validation layers by the system.
 */
inline std::vector<LayerProperties> get_supported_validation_layers() {
  std::uint32_t layer_count {};
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr) >> check::error;

  std::vector<LayerProperties> availableLayers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, availableLayers.data()) >> check::error;

  for (auto const& layer: availableLayers) {
    LOG_INFO("Vulkan Validation Layer: {} (version {})", layer.layerName, layer.specVersion);
  }

  return availableLayers;
}

/**
 * Reflect3d validation layers setup.
 *
 * Verifies if the requested validation layers are supported by the current system
 * and if they are not enabled, returns an empty vector.
 *
 * @return expected ValidationLayers if validation layers are enabled and supported.
 * by the current system.
 */
inline mono::err::expected<std::vector<char const*>> get_validation_layers() {
  if constexpr (not enable_validation_layers) {
    LOG_INFO("Validation layers are disabled");
    return std::vector<char const*> {};
  }

  LOG_INFO("Validation layers are enabled");

  static std::vector<std::string_view> const validation_layers = {
    "VK_LAYER_KHRONOS_validation",
  };

  if (not check_validation_layer_support(get_supported_validation_layers(), validation_layers)) {
    return mono::err::unexpected("Validation layers requested, but not all are available.");
  }

  return validation_layers //
         | std::views::transform([](auto const& layer) { return layer.data(); }) //
         | std::ranges::to<std::vector>();
}

} // namespace rf3d::hri::vk
