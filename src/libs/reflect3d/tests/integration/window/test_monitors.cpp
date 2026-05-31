
#include "reflect3d/window/monitor.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

int main() try { //
  // if (glfwInit() == 0) {
  //   throw std::runtime_error("Couldn't initilize glfw");
  // };
  rf3d::log_monitors();
}
catch (std::exception const& e) {
  std::cerr << "An error occurred while logging monitors: " << e.what() << '\n';
}
catch (...) {
  std::cerr << "An error occurred while logging monitors\n";
}
