
#include "common.hpp"
#include "reflect3d/window/monitor.hpp"

#include <GLFW/glfw3.h>

int main() try { //
  // if (glfwInit() == 0) {
  //   throw std::runtime_error("Couldn't initilize glfw");
  // };
  rf3d::log_monitors();
}
catch (...) {
  std::println("Something went wrong...");
}
