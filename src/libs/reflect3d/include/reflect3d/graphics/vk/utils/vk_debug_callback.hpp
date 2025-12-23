#pragma once

#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

#include <quill/core/LogLevel.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {

inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, //
    VkDebugUtilsMessageTypeFlagsEXT messageType, //
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, //
    void* pUserData //
) {
  // clang-format off
  static mono::unordered_flat_map<VkDebugUtilsMessageSeverityFlagBitsEXT, quill::LogLevel> const severity_map {
    {VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,   mono::LogLevel::Info},
    {VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,      mono::LogLevel::Info},
    {VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,   mono::LogLevel::Warning},
    {VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,     mono::LogLevel::Error},
  }; 

  static mono::unordered_flat_map<VkDebugUtilsMessageTypeFlagsEXT, std::string_view> const type_map {
    {VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,      "GENERAL"},
    {VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,   "VALIDATION"},
    {VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,  "PERFORMANCE"},
  };
  // clang-format on

  LOG_DYNAMIC(severity_map.at(messageSeverity), "[Vulkan {}] {}", type_map.at(messageType), pCallbackData->pMessage);

  return VK_FALSE;
}


} // namespace rf3d::hri::vk
