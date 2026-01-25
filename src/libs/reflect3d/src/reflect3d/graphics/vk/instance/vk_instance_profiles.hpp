#pragma once


#include "reflect3d/graphics/vk/instance/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/instance/vk_extensions.hpp"
#include "reflect3d/window/utils/context.hpp"

#include <mono/logging/logger.hpp>

namespace rf3d::gfx::vk::detail {

/**
 * Creates a native Vulkan instance based on the provided configuration.
 *
 * @note may throw if mandatory extensions are not supported
 * @return A NativeInstance handle representing the created Vulkan instance.
 */
raii::Instance create_instance(raii::Context const& context) {
  LOG_INFO("Initializing Vulkan instance");
  platform::init();
  static constexpr core::ApplicationInfo app_info {
    .pApplicationName   = "Reflect3D Application",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName        = "Reflect3D Engine",
    .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion         = core::ApiVersion14,
  };

  static auto extensions = get_extensions(context).value();

  core::InstanceCreateInfo create_info {
    .pApplicationInfo        = &app_info,
    .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
    .ppEnabledExtensionNames = extensions.data(),
  };

  setup_validation_layers(context, create_info);

  return {context, create_info};
}

struct ReleaseInstance {
  /*********************
   *    Type Traits    *
   ********************/

  using native_type = raii::Instance;

  /**********************
   *    Constructors    *
   **********************/

  explicit ReleaseInstance(raii::Context const& context) : handle(detail::create_instance(context)) { }

  native_type handle;
};

/**
 * Instance with debug messenger this will be used
 * when Vulkan validation layers are enabled
 */
struct DebugInstance {
  /*********************
   *    Type Traits    *
   ********************/

  using messenger_type = raii::DebugUtilsMessengerEXT;
  using native_type    = raii::Instance;


  /**********************
   *    Constructors    *
   **********************/

  DebugInstance(raii::Context const& context) :
    handle(detail::create_instance(context)),
    debug_messenger(handle.createDebugUtilsMessengerEXT(debug_utils_messenger_create_info)) { }

  native_type handle;
  messenger_type debug_messenger;
};

} // namespace rf3d::gfx::vk::detail
