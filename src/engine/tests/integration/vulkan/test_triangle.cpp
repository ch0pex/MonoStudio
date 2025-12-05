

#include <rflect3d/core/execution/signals.hpp>
#include <rflect3d/core/execution/stop_token.hpp>
#include <rflect3d/input/input.hpp>
#include <rflect3d/window/window.hpp>
#include <rflect3d/window/window_builder.hpp>
#include <rflect3d/window/window_config.hpp>
#include <rflect3d/window/window_types.hpp>
#include "rflect3d/graphics/vk/vk_instance.hpp"


int main() {
  rflect::config::Window window_spec {
    .mode = rflect::WindowMode::windowed,
  };


  rflect::gfx::vk::Instance vulkan {};
  rflect::Window window {rflect::WindowBuilder(window_spec).default_callbacks().build()};

  rflect::ex::setup_signals();

  while (rflect::ex::should_run()) {
    rflect::input::poll_events();
    glfwSwapBuffers(window.native_handle());
  }
}
