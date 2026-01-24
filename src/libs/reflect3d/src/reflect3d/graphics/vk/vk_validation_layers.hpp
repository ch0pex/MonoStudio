#pragma once

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"


// Mono library
#include <mono/logging/logger.hpp>

// STD library
#include <algorithm>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <vector>

namespace rf3d::gfx::vk {

#ifdef NDEBUG
inline constexpr bool enable_validation_layers = false;
#else
inline constexpr bool enable_validation_layers = true;
#endif


/**
 * Given a list of supported validation layers and required validation layers,
 * returns true if all required layers are supported.
 *
 * @return true if all requested validation layers are supported.
 */
template <std::ranges::range SupportedLayers, std::ranges::range RequiredLayers>
  requires(std::same_as<std::ranges::range_value_t<SupportedLayers>, core::LayerProperties> and
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
inline std::vector<core::LayerProperties> get_supported_validation_layers(raii::Context const& context) {

  auto const availableLayers = context.enumerateInstanceLayerProperties();

  for (auto const& layer: availableLayers) {
    LOG_INFO("Vulkan Validation Layer: {} (version {})", layer.layerName.data(), layer.specVersion);
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
 * by the current system. Empty vector otherwise.
 *
 */
inline std::vector<char const*> get_validation_layers(raii::Context const& context) {
  if constexpr (not enable_validation_layers) {
    LOG_INFO("Validation layers are disabled");
    return {};
  }

  LOG_INFO("Validation layers are enabled");

  static std::vector<std::string_view> const validation_layers = {
    "VK_LAYER_KHRONOS_validation",
  };

  if (not check_validation_layer_support(get_supported_validation_layers(context), validation_layers)) {
    LOG_WARNING(
        "Validation layers were requested, but not all are available. Program execution may continue without them"
    );
    return {};
  }

  return validation_layers //
         | std::views::transform([](auto const& layer) { return layer.data(); }) //
         | std::ranges::to<std::vector>();
}

} // namespace rf3d::gfx::vk
