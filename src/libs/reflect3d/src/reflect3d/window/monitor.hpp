#pragma once

#include "reflect3d/math/math.hpp"
#include "reflect3d/window/utils/context.hpp"
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

struct MonitorWorkingArea {
  math::u16vec2 position {};
  Resolution size {};
};

class Monitor {
  explicit Monitor(NativeMonitor const monitor) : handle(monitor) { }

public:
  // --- Type traits ---

  using native_type       = NativeMonitor;
  using vidmode_type      = GLFWvidmode const*;
  using working_area_type = MonitorWorkingArea;

  // --- Factory methods ---

  static std::optional<Monitor> from_id(MonitorId const monitor_id) {
    platform::init();
    auto const monitors = detail::get_native_monitors();

    if (monitor_id >= monitors.size()) {
      LOG_WARNING("Specified monitor doesn't exist")
      return std::nullopt;
    }
    return {Monitor {monitors[monitor_id]}};
  }

  static Monitor primary() {
    platform::init();
    auto* monitor = glfwGetPrimaryMonitor();
    return Monitor {monitor};
  }

  static std::vector<Monitor> get_all() {
    platform::init();
    return detail::get_native_monitors() //
           | std::views::transform([](NativeMonitor monitor) { return Monitor {monitor}; }) //
           | std::ranges::to<std::vector<Monitor>>();
  }

  // --- Member functions ---

  [[nodiscard]] std::string name() const { return glfwGetMonitorName(handle); }

  void attach_window(NativeWindow const window) const {
    glfwSetWindowMonitor(
        window, handle, 0, 0, physical_resolution().width, physical_resolution().height, refresh_rate()
    );
  }

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

    auto const width  = static_cast<std::uint16_t>(static_cast<float>(phys.width) / scale.x);
    auto const height = static_cast<std::uint16_t>(static_cast<float>(phys.height) / scale.y);

    return {.width = width, .height = height};
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

  [[nodiscard]] std::uint8_t refresh_rate() const {
    auto const* mode = glfwGetVideoMode(handle);
    return mode->refreshRate;
  }

  [[nodiscard]] working_area_type working_area() const {
    int x {};
    int y {};
    int width {};
    int height {};
    glfwGetMonitorWorkarea(handle, &x, &y, &width, &height);

    return {
      {static_cast<std::uint16_t>(x), static_cast<std::uint16_t>(y)},
      {.width = static_cast<std::uint16_t>(width), .height = static_cast<std::uint16_t>(height)}
    };
  }

  [[nodiscard]] native_type native_handle() const { return handle; }

private:
  native_type handle {};
};


inline void log_monitors() {
  auto monitors = Monitor::get_all();
  LOG_INFO("Monitors count: {}", monitors.size());

  for (auto monitor: monitors) {
    LOG_INFO("Selected Monitor: {}", monitor.name());
    LOG_INFO(
        "Logical resolution: Width: {}, Height: {}", //
        monitor.logical_resolution().width, //
        monitor.logical_resolution().height //
    )
    LOG_INFO("Scale: X: {}, Y: {}", monitor.content_scale().x, monitor.content_scale().y)

    LOG_INFO(
        "Physical resolution: Width: {}, Height: {}", //
        monitor.physical_resolution().width, //
        monitor.physical_resolution().height
    )

    auto const pos = monitor.position();
    LOG_INFO("Monitior pos: {}, {}", pos.x, pos.y);
  }
}

} // namespace rf3d
