
#include <iostream>
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"


int main(int argc, char** argv) try { //
  mono::span const args {argv, static_cast<std::size_t>(argc)};
  auto id   = argc > 1 ? static_cast<rf3d::MonitorId>(std::strtol(args[1], nullptr, 10)) : rf3d::MonitorId {0};
  auto mode = argc > 2 ? static_cast<rf3d::WindowMode>(std::strtol(args[2], nullptr, 10))
                       : rf3d::WindowMode::borderless_full_screen;

  test_window(mode, id);
}
catch (std::exception const& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unknown error occurred" << '\n';
  return EXIT_FAILURE;
}
