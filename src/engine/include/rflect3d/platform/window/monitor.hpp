#pragma once

#include "rflect3d/core/logging/logger.hpp"
#include "rflect3d/platform/window/window_types.hpp"

#include <optional>
#include <span>
#include <stdexcept>

namespace rflect {

inline std::span<Monitor> get_monitors() {
  int count {0};
  auto* monitors = glfwGetMonitors(&count);
  if (count == 0) {
    throw std::runtime_error("The platform layer GLFW could not detect any monitors");
  }

  return {monitors, static_cast<std::size_t>(count)};
}

inline std::optional<Monitor> get_monitor(MonitorId const monitor_id) {
  auto monitors = get_monitors();

  if (monitors.size() < monitor_id) {
    LOG_WARNING("Specified monitor doesn't exist")
    return std::nullopt;
  }
  return monitors[monitor_id];
}

} // namespace rflect
