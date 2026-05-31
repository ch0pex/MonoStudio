/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_checker.hpp
 * @date 26/05/2026
 * @brief Vulkan result checking utilities with error and warning handlers
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/utils/vk_exception.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_result.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <source_location>

// --- System ---


namespace rf3d::vk::detail {

struct Error {
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  Error(core::Result result, std::source_location const& location = std::source_location::current()) noexcept :
    result(result), location(location) { };
  core::Result result;
  std::source_location location;
};

struct ErrorGrabber { };

struct WarningGrabber { };

inline void operator>>(Error const& grabber, [[maybe_unused]] ErrorGrabber const checker) {
  if (grabber.result != core::Result::eSuccess and grabber.result != core::Result::eSuboptimalKHR) {
    std::string const error[[]] = std::format(
        "{}:{}:{}: VkResult failed with error code {} ({})", grabber.location.file_name(), grabber.location.line(),
        grabber.location.column(), to_string(grabber.result), error_message(grabber.result)
    );
    throw Exception(error);
  }
}

inline void operator>>(Error const& grabber, [[maybe_unused]] WarningGrabber const checker) {
  if (grabber.result != core::Result::eSuccess and grabber.result != core::Result::eSuboptimalKHR) {
    std::string const error = std::format(
        "{}:{}:{}: VkResult failed with error code {} ({})", grabber.location.file_name(), grabber.location.line(),
        grabber.location.column(), to_string(grabber.result), error_message(grabber.result)
    );
    LOG_WARNING("{}", error);
  }
}

namespace check {

inline constexpr ErrorGrabber error;

inline constexpr WarningGrabber warn;

} // namespace check

} // namespace rf3d::vk::detail
