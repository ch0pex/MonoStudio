#pragma once

//
#include "reflect3d/graphics/vk/utils/vk_debug_callback.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

//
#include <mono/containers/unordered_flat_map.hpp>
#include <mono/logging/logger.hpp>

namespace rf3d::gfx::vk {


core::DebugUtilsMessageSeverityFlagsEXT constexpr severity_flags(
    core::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose //
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

} // namespace rf3d::gfx::vk
