#pragma once

#include <mono/misc/passkey.hpp>

//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace rf3d {

class WindowBuilder;

/**
 * Lazy glfw's context initilizer
 * This can only be instantiated by window class
 */
struct GlfwContext {
  GlfwContext(GlfwContext&&)                 = delete;
  GlfwContext& operator=(GlfwContext&&)      = delete;
  GlfwContext(GlfwContext const&)            = delete;
  GlfwContext& operator=(GlfwContext const&) = delete;
  ~GlfwContext() { glfwTerminate(); }

  GlfwContext(mono::PassKey<WindowBuilder> key [[maybe_unused]]) {
    if (glfwInit() == 0) {
      throw std::runtime_error("Couldn't initilize glfw");
    };
  }
};

} // namespace rf3d
