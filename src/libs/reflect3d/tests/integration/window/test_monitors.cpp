
#include "common.hpp"
#include "reflect3d/window/monitor.hpp"

#include <GLFW/glfw3.h>

int main() try { //
  if (glfwInit() == 0) {
    throw std::runtime_error("Couldn't initilize glfw");
  };

  auto monitors = rf3d::get_monitors();
  LOG_INFO("Monitors count: {}", monitors.size());

  for (auto monitor: monitors) {
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
catch (...) {
  std::println("Something went wrong...");
}
