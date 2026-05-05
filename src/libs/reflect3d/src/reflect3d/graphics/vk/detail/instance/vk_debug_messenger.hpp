#pragma once

//
#include "reflect3d/graphics/vk/detail/utils/vk_debug_callback.hpp"

namespace rf3d::vk::detail {

core::DebugUtilsMessageSeverityFlagsEXT constexpr severity_flags(
    core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose //
    | core::DebugUtilsMessageSeverityFlagBitsEXT::eInfo //
    | core::DebugUtilsMessageSeverityFlagBitsEXT::eWarning //
    | core::DebugUtilsMessageSeverityFlagBitsEXT::eError //
);
core::DebugUtilsMessageTypeFlagsEXT constexpr message_type_flags(
    core::DebugUtilsMessageTypeFlagBitsEXT::eGeneral //
    | core::DebugUtilsMessageTypeFlagBitsEXT::ePerformance //
    | core::DebugUtilsMessageTypeFlagBitsEXT::eValidation //
);

core::DebugUtilsMessengerCreateInfoEXT constexpr debug_utils_messenger_create_info {
  .messageSeverity = severity_flags,
  .messageType     = message_type_flags,
  .pfnUserCallback = &debug_callback,
};

} // namespace rf3d::vk::detail
