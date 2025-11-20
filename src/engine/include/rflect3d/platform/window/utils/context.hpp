#pragma once

#include "rflect3d/core/misc/passkey.hpp"

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
  GlfwContext(PassKey<WindowBuilder> key [[maybe_unused]]) {
    if (not glfwInit()) {
      throw std::runtime_error("Couldn't initilize glfw");
    };
  }

  GlfwContext(GlfwContext const&) = delete;

  GlfwContext& operator=(GlfwContext const&) = delete;

  ~GlfwContext() { glfwTerminate(); }
};

} // namespace rflect
