#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

#include <quill/core/LogLevel.h>
#include <vulkan/vulkan_enums.hpp>

namespace rf3d::gfx::vk {

// inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
//     core::DebugUtilsMessageSeverityFlagBitsEXT message_severity, //
//     core::DebugUtilsMessageTypeFlagsEXT message_type, //
//     core::DebugUtilsMessengerCallbackDataEXT const* callback_data, //
//     void* user_data //
// ) {
//   // clang-format off
//   static mono::unordered_flat_map<core::DebugUtilsMessageSeverityFlagBitsEXT, quill::LogLevel> const severity_map {
//     {core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,   mono::LogLevel::Info},
//     {core::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,      mono::LogLevel::Info},
//     {core::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,   mono::LogLevel::Warning},
//     {core::DebugUtilsMessageSeverityFlagBitsEXT::eError,     mono::LogLevel::Error},
//   };
//
//   static mono::unordered_flat_map<core::DebugUtilsMessageTypeFlagBitsEXT, std::string_view> const type_map {
//     {core::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,      "GENERAL"},
//     {core::DebugUtilsMessageTypeFlagBitsEXT::eValidation,   "VALIDATION"},
//     {core::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,  "PERFORMANCE"},
//   };
//   // clang-format on
//
//   core::DebugUtilsMessageTypeFlagBitsEXT const type_key {
//     static_cast<core::DebugUtilsMessageTypeFlagsEXT::MaskType>(message_type)
//   };
//
//   std::string_view const type_string = type_map.at(type_key);
//   LOG_DYNAMIC(severity_map.at(message_severity), "[Vulkan {}] {}", type_string, callback_data->pMessage);
//
//   return core::False;
// }

inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    core::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
    core::DebugUtilsMessageTypeFlagsEXT message_type, // Nota: Flags, no FlagBits
    core::DebugUtilsMessengerCallbackDataEXT const* callback_data, void* user_data
) {
  static mono::unordered_flat_map<core::DebugUtilsMessageSeverityFlagBitsEXT, quill::LogLevel> const severity_map {
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose, mono::LogLevel::Debug},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eInfo, mono::LogLevel::Info},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eWarning, mono::LogLevel::Warning},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eError, mono::LogLevel::Error},
  };

  auto const it_sev           = severity_map.find(message_severity);
  quill::LogLevel const level = (it_sev != severity_map.end()) ? it_sev->second : mono::LogLevel::Critical;

  std::string type_string;
  if (message_type & core::DebugUtilsMessageTypeFlagBitsEXT::eGeneral)
    type_string += "GENERAL ";
  if (message_type & core::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
    type_string += "VALIDATION ";
  if (message_type & core::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
    type_string += "PERFORMANCE ";

  if (type_string.empty())
    type_string = "UNKNOWN";
  else
    type_string.pop_back();

  LOG_DYNAMIC(level, "[Vulkan {}] {}", type_string, callback_data->pMessage);

  return core::False;
}


} // namespace rf3d::gfx::vk
