/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_debug_messenger.hpp
 * @date 26/05/2026
 * @brief Vulkan debug messenger configuration
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/utils/vk_debug_callback.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


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
