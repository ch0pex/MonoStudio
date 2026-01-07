#pragma once

#include "reflect3d/graphics/vk/utils/vk_exception.hpp"
#include "reflect3d/graphics/vk/utils/vk_result.hpp"

#include <source_location>

namespace rf3d::gfx::vk {

struct Error {
  Error(core::Result result, std::source_location const& location = std::source_location::current()) noexcept :
    result(result), location(location) { };
  core::Result result;
  std::source_location location;
};

struct ErrorGrabber { };

struct WarningGrabber { };

inline void operator>>(Error const& grabber, ErrorGrabber const checker) {
  if (grabber.result != core::Result::eSuccess and grabber.result != core::Result::eSuboptimalKHR) {
    std::string const error = std::format(
        "{}:{}:{}: VkResult failed with error code {} ({})", grabber.location.file_name(), grabber.location.line(),
        grabber.location.column(), to_string(grabber.result), error_message(grabber.result)
    );
    throw Exception(error);
  }
}

inline void operator>>(Error const& grabber, WarningGrabber const checker) {
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

} // namespace rf3d::gfx::vk
