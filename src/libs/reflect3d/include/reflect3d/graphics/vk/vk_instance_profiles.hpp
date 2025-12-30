#pragma once


#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_debug_callback.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/vk_instance_detail.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace rf3d::hri::vk::detail {

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

} // namespace rf3d::hri::vk::detail
