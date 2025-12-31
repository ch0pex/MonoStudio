#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

#include <quill/core/LogLevel.h>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::gfx::vk {

inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    core::DebugUtilsMessageSeverityFlagBitsEXT message_severity, //
    core::DebugUtilsMessageTypeFlagsEXT message_type, //
    core::DebugUtilsMessengerCallbackDataEXT const* callback_data, //
    void* user_data //
) {
  // clang-format off
  static mono::unordered_flat_map<core::DebugUtilsMessageSeverityFlagBitsEXT, quill::LogLevel> const severity_map {
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,   mono::LogLevel::Info},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,      mono::LogLevel::Info},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,   mono::LogLevel::Warning},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eError,     mono::LogLevel::Error},
  }; 

  static mono::unordered_flat_map<core::DebugUtilsMessageTypeFlagBitsEXT, std::string_view> const type_map {
    {core::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,      "GENERAL"},
    {core::DebugUtilsMessageTypeFlagBitsEXT::eValidation,   "VALIDATION"},
    {core::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,  "PERFORMANCE"},
  };
  // clang-format on

  core::DebugUtilsMessageTypeFlagBitsEXT const type_key {
    static_cast<core::DebugUtilsMessageTypeFlagsEXT::MaskType>(message_type)
  };

  LOG_DYNAMIC(severity_map.at(message_severity), "[Vulkan {}] {}", type_map.at(type_key), callback_data->pMessage);

  return core::False;
}


} // namespace rf3d::gfx::vk
