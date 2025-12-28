
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"


int main(int argc, char** argv) { //
  rf3d::config::Window window_spec {.mode = rf3d::WindowMode::windowed};
  rf3d::Window window {rf3d::WindowBuilder(window_spec).default_callbacks().build()};

  mono::ex::setup_signals();
  rf3d::log_monitors();

  glfwMakeContextCurrent(window);


  // -------- Initial State --------
  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Change to Fullscreen --------

  window.set_full_screen(0);

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Change to Windowed --------

  window.windowed();

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Hide decorations  --------

  window.hide_decorations();

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // -------- Show decorations  --------

  window.show_decorations();

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // ---- Hide  ----

  window.hide();

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // ---- Show  ----
  window.show();

  rf3d::input::poll_events();
  glfwSwapBuffers(window);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
