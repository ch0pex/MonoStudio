
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"

int main(int argc, char** argv) { //
  rflect::MonitorId id = argc > 1 ? std::atoi(argv[1]) : 0;
  auto mode =
      argc > 2 ? static_cast<rflect::WindowMode>(std::atoi(argv[2])) : rflect::WindowMode::borderless_full_screen;

  test_window(mode, id);
}
