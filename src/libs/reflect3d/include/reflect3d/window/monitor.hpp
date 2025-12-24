#pragma once

#include "reflect3d/math/math.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window_types.hpp"

#include <mono/logging/logger.hpp>

#include <GLFW/glfw3.h>
#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>

namespace rf3d {

namespace detail {

inline std::span<NativeMonitor> get_native_monitors() {
  int count {0};
  auto* monitors = glfwGetMonitors(&count);
  if (count == 0) {
    throw std::runtime_error("The platform layer GLFW could not detect any monitors");
  }

  return {monitors, static_cast<std::size_t>(count)};
}

} // namespace detail


class Monitor {
public:
  // --- Type traits ---
  using native_type  = NativeMonitor;
  using vidmode_type = GLFWvidmode const*;

  // --- Constructors ---
  Monitor(NativeMonitor const monitor) : handle(monitor) { }

  static std::optional<Monitor> from_id(MonitorId const monitor_id) {
    auto const monitors = detail::get_native_monitors();

    if (monitor_id >= monitors.size()) {
      LOG_WARNING("Specified monitor doesn't exist")
      return std::nullopt;
    }
    return monitors[monitor_id];
  }

  static Monitor primary() { return glfwGetPrimaryMonitor(); }

  // --- Member functions ---

  [[nodiscard]] std::string name() const { return glfwGetMonitorName(handle); }

  [[nodiscard]] Resolution physical_resolution() const {
    auto const* mode = glfwGetVideoMode(handle);
    return {.width = static_cast<uint16_t>(mode->width), .height = static_cast<uint16_t>(mode->height)};
  }

  [[nodiscard]] math::vec2 content_scale() const {
    float x_scale {};
    float y_scale {};
    glfwGetMonitorContentScale(handle, &x_scale, &y_scale);
    return {x_scale, y_scale};
  }

  [[nodiscard]] Resolution logical_resolution() const {
    auto const phys  = physical_resolution();
    auto const scale = content_scale();
    if (scale.x == 0 or scale.y == 0)
      return phys;

    return {
      .width = static_cast<uint16_t>(phys.width / scale.x), .height = static_cast<uint16_t>(phys.height / scale.y)
    };
  }

  [[nodiscard]] math::vec<2, int> position() const {
    int x {};
    int y {};
    glfwGetMonitorPos(handle, &x, &y);
    return {x, y};
  }

  [[nodiscard]] std::uint8_t redBits() const {
    auto const* mode = glfwGetVideoMode(handle);
    return mode->redBits;
  }

  [[nodiscard]] std::uint8_t greenBits() const {
    auto const* mode = glfwGetVideoMode(handle);
    return mode->greenBits;
  }

  [[nodiscard]] std::uint8_t blueBits() const {
    auto const* mode = glfwGetVideoMode(handle);
    return mode->blueBits;
  }

  [[nodiscard]] std::uint8_t refreshRate() const {
    auto const* mode = glfwGetVideoMode(handle);
    return mode->refreshRate;
  }

  [[nodiscard]] native_type native_handle() const { return handle; }

private:
  native_type handle {};
};


inline std::vector<Monitor> get_monitors() {
  return detail::get_native_monitors() //
         | std::views::transform([](NativeMonitor monitor) { return Monitor {monitor}; }) //
         | std::ranges::to<std::vector<Monitor>>();
}


} // namespace rf3d
