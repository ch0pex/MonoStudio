#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_debug_callback.hpp"

//
#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

//
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {


namespace detail {

inline VkResult create_debug_utils_messenger_ext(
    VkInstance instance, //
    VkDebugUtilsMessengerCreateInfoEXT const* pCreateInfo, //
    VkAllocationCallbacks const* pAllocator, //
    VkDebugUtilsMessengerEXT* pDebugMessenger //
) {
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( // NOLINT
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
  );

  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }

  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

inline void destroy_debug_util_messenger_ext(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, VkAllocationCallbacks const* pAllocator
) {
  auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>( // NOLINT
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
  );

  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

} // namespace detail

using DebugUtilsCreateInfo = VkDebugUtilsMessengerCreateInfoEXT;

inline DebugUtilsCreateInfo const debug_utils_create_info = {
  .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
  .pNext           = nullptr,
  .flags           = {},
  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT //
                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT //
                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, //
  .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT //
                 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT //
                 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
  .pfnUserCallback = debug_callback,
  .pUserData       = nullptr,
}; // namespace rf3d::hri::vk


} // namespace rf3d::hri::vk
