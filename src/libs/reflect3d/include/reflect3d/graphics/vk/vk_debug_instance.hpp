#pragma once

#include "reflect3d/graphics/vk/vk_base_instance.hpp"


namespace rf3d::hri::vk {

/**
 * Instance with debug messenger this will be used
 * when Vulkan validation layers are enabled
 */
struct DebugInstance final : private BaseInstance {

  using BaseInstance::handle;
  using messenger_type = VkDebugUtilsMessengerEXT;

  DebugInstance(InstanceConfig const& config = {}) : BaseInstance(config) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debug_callback;

    detail::create_debug_utils_messenger_ext(this->handle, &createInfo, nullptr, &debug_messenger) >> check::error;
  }

  ~DebugInstance() {
    if (debug_messenger != VK_NULL_HANDLE) {
      detail::destroy_debug_util_messenger_ext(this->handle, debug_messenger, nullptr);
    }
  }

  DebugInstance(DebugInstance const&) = delete;

  DebugInstance& operator=(DebugInstance const&) = delete;

  DebugInstance(DebugInstance&& other) noexcept :
    BaseInstance(std::move(other)), debug_messenger(std::exchange(other.debug_messenger, VK_NULL_HANDLE)) { }

  DebugInstance& operator=(DebugInstance&& other) noexcept {

    if (this != &other) {
      if (debug_messenger != VK_NULL_HANDLE) {
        detail::destroy_debug_util_messenger_ext(this->handle, debug_messenger, nullptr);
      }

      BaseInstance::operator=(std::move(other));
      debug_messenger = std::exchange(other.debug_messenger, VK_NULL_HANDLE);
    }
    return *this;
  }

  messenger_type debug_messenger {VK_NULL_HANDLE};
};

} // namespace rf3d::hri::vk
