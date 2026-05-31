
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"

#include <iostream>
#include <thread>

namespace {

void make_context_current(rf3d::Window& window) { glfwMakeContextCurrent(window.handle()); }

} // namespace

int main() try { //
  rf3d::config::Window const window_spec {.mode = rf3d::WindowMode::windowed};
  rf3d::Window window {rf3d::WindowBuilder(window_spec).default_callbacks().build()};

  mono::ex::setup_signals();
  rf3d::log_monitors();

  make_context_current(window);

  // -------- Initial State --------
  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Change to Fullscreen --------

  window.set_full_screen(0);

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Change to Windowed --------

  window.windowed();

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Hide decorations  --------

  window.hide_decorations();

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Show decorations  --------

  window.show_decorations();

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // ---- Hide  ----

  window.hide();

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // ---- Show  ----
  window.show();

  rf3d::input::poll_events();
  window.swap_buffers();

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
catch (std::exception const& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unknown error occurred" << '\n';
  return EXIT_FAILURE;
}
