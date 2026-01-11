#pragma once

#include <mono/misc/passkey.hpp>

#include <GLFW/glfw3.h>

#ifdef MONO_USE_CPP_MODULES
#else
#include <stdexcept>
#endif


namespace rf3d {

class GlfwContext;

namespace detail {

struct ContextInstance {
  ContextInstance(ContextInstance&&)                 = delete;
  ContextInstance& operator=(ContextInstance&&)      = delete;
  ContextInstance(ContextInstance const&)            = delete;
  ContextInstance& operator=(ContextInstance const&) = delete;
  ~ContextInstance() { glfwTerminate(); }

  ContextInstance(mono::PassKey<GlfwContext> key [[maybe_unused]]) {
    if (glfwInit() == GLFW_FALSE) {
      throw std::runtime_error("Couldn't initilize glfw");
    };
  }
};

} // namespace detail

/**
 * Lazy glfw's context initilizer
 * This can only be instantiated by window class
 */
class GlfwContext {
  GlfwContext() = default;

  ~GlfwContext() = default;

public:
  GlfwContext(GlfwContext&&) = delete;

  GlfwContext& operator=(GlfwContext&&) = delete;

  explicit GlfwContext(GlfwContext const&) = delete;

  GlfwContext& operator=(GlfwContext const&) = delete;

  static void init() { static detail::ContextInstance instance {mono::PassKey<GlfwContext> {}}; }
};

} // namespace rf3d
