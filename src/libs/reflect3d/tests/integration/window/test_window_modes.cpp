
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"


int main(int argc, char** argv) { //
  rf3d::MonitorId id = argc > 1 ? std::atoi(argv[1]) : 1;
  auto mode = argc > 2 ? static_cast<rf3d::WindowMode>(std::atoi(argv[2])) : rf3d::WindowMode::borderless_full_screen;

  test_window(mode, id);
}
