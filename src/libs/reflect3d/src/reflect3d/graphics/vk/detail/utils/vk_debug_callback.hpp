#pragma once

#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"

#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

namespace rf3d::vk::detail {

inline VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    core::DebugUtilsMessageSeverityFlagBitsEXT message_severity,
    core::DebugUtilsMessageTypeFlagsEXT message_type, // Nota: Flags, no FlagBits
    core::DebugUtilsMessengerCallbackDataEXT const* callback_data, //
    [[maybe_unused]] void* user_data
) {
  static mono::unordered_flat_map<core::DebugUtilsMessageSeverityFlagBitsEXT, mono::LogLevel> const severity_map {
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose, mono::LogLevel::debug},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eInfo, mono::LogLevel::info},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eWarning, mono::LogLevel::warning},
    {core::DebugUtilsMessageSeverityFlagBitsEXT::eError, mono::LogLevel::error},
  };

  auto const it_sev          = severity_map.find(message_severity);
  mono::LogLevel const level = (it_sev != severity_map.end()) ? it_sev->second : mono::LogLevel::critical;

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


} // namespace rf3d::vk::detail
