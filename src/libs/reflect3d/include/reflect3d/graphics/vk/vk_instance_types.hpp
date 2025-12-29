#pragma once


#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_debug_callback.hpp"
#include "reflect3d/graphics/vk/vk_debug_messenger.hpp"
#include "reflect3d/graphics/vk/vk_instance_detail.hpp"

#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk::detail {

class ReleaseInstance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using handle_type = VkInstance;

  // using messenger_type = Dew

  /**********************
   *    Constructors    *
   **********************/

  explicit ReleaseInstance() : handle(detail::create_instance()) { }

  ~ReleaseInstance() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyInstance(handle, nullptr);
      handle = VK_NULL_HANDLE;
    }
  }

  ReleaseInstance(ReleaseInstance const&) = delete;

  ReleaseInstance& operator=(ReleaseInstance const&) = delete;

  ReleaseInstance(ReleaseInstance&& other) noexcept : handle(std::exchange(other.handle, VK_NULL_HANDLE)) { }

  ReleaseInstance& operator=(ReleaseInstance&& other) noexcept {
    if (this != std::addressof(other)) {
      if (handle != VK_NULL_HANDLE) {
        vkDestroyInstance(handle, nullptr);
      }
      handle = std::exchange(other.handle, VK_NULL_HANDLE);
    }
    return *this;
  }

  /**************************
   *       Operators        *
   **************************/

  [[nodiscard]] operator handle_type() const { return handle; }

private:
  handle_type handle;
};

/**
 * Instance with debug messenger this will be used
 * when Vulkan validation layers are enabled
 */
class DebugInstance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using messenger_type = VkDebugUtilsMessengerEXT;
  using handle_type    = VkInstance;

  /**********************
   *    Constructors    *
   **********************/

  DebugInstance() {
    detail::create_debug_utils_messenger_ext(this->handle, &debug_utils_create_info, nullptr, &debug_messenger) >>
        check::error;
  }

  ~DebugInstance() {
    if (debug_messenger != VK_NULL_HANDLE) {
      detail::destroy_debug_util_messenger_ext(this->handle, debug_messenger, nullptr);
    }
  }

  DebugInstance(DebugInstance const&) = delete;

  DebugInstance& operator=(DebugInstance const&) = delete;

  DebugInstance(DebugInstance&& other) noexcept :
    debug_messenger(std::exchange(other.debug_messenger, VK_NULL_HANDLE)) { }

  DebugInstance& operator=(DebugInstance&& other) noexcept {

    if (this != &other) {
      if (debug_messenger != VK_NULL_HANDLE) {
        detail::destroy_debug_util_messenger_ext(this->handle, debug_messenger, nullptr);
      }

      handle          = std::exchange(other.handle, VK_NULL_HANDLE);
      debug_messenger = std::exchange(other.debug_messenger, VK_NULL_HANDLE);
    }
    return *this;
  }

  /**************************
   *       Operators        *
   **************************/

  [[nodiscard]] operator handle_type() const { return handle; }

private:
  handle_type handle {detail::create_instance()};
  messenger_type debug_messenger {VK_NULL_HANDLE};
};

} // namespace rf3d::hri::vk::detail
