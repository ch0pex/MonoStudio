
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
    LOG_INFO("Width: {}, Height: {}", monitor.resolution().width, monitor.resolution().height)
    auto pos = monitor.position();
    LOG_INFO("Montior pos: {}, {}", pos.x, pos.y);
  }
}
catch (...) {
  std::println("Something went wrong...");
}
