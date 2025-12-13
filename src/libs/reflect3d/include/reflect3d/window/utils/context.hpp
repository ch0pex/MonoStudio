#pragma once

#include <mono-core/misc/passkey.hpp>

//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace rflect {

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

  GlfwContext(PassKey<WindowBuilder> key [[maybe_unused]]) {
    if (glfwInit() == 0) {
      throw std::runtime_error("Couldn't initilize glfw");
    };
  }
};

} // namespace rflect
