#pragma once

#include "reflect3d/graphics/vk/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/vk_extensions.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"


namespace rf3d::hri::vk::detail {

/**
 * Setups validation layers and debug utils info
 */
inline void setup_validation_layers(raii::Context const& context, core::InstanceCreateInfo& create_info) {
  if constexpr (enable_validation_layers) {
    // This needs to survive until vkCreateInstance is invoked thats why it has static storage
    static auto const validation_layers = get_validation_layers(context);
    create_info.enabledLayerCount       = validation_layers.size();
    create_info.ppEnabledLayerNames     = not validation_layers.empty() ? validation_layers.data() : nullptr;
    create_info.pNext                   = &debug_utils_messenger_create_info;
  }
}

/**
 * Creates a native Vulkan instance based on the provided configuration.
 *
 * @note may throw if mandatory extensions are not supported
 * @return A NativeInstance handle representing the created Vulkan instance.
 */
inline raii::Instance create_instance(raii::Context const& context) {
  static constexpr core::ApplicationInfo app_info {
    .pApplicationName   = "Reflect3D Application",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName        = "Reflect3D Engine",
    .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion         = core::ApiVersion14,
  };

  auto extensions = get_extensions(context).value();

  core::InstanceCreateInfo create_info {
    .pApplicationInfo        = &app_info,
    .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
  };

  setup_validation_layers(context, create_info);

  return {context, create_info};
}


} // namespace rf3d::hri::vk::detail
