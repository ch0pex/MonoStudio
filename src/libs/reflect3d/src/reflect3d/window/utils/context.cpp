#include "reflect3d/window/utils/context.hpp"

#include <mono/misc/passkey.hpp>
//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace rf3d::platform {

namespace {

struct ContextInstance {
  explicit ContextInstance() {
    if (glfwInit() == GLFW_FALSE) {
      throw std::runtime_error("Couldn't initilize glfw");
    }
  }

  ~ContextInstance() { glfwTerminate(); }

  ContextInstance(ContextInstance&&) = delete;

  ContextInstance& operator=(ContextInstance&&) = delete;

  ContextInstance(ContextInstance const&) = delete;

  ContextInstance& operator=(ContextInstance const&) = delete;
};

} // namespace

void init() { static ContextInstance instance {}; }

} // namespace rf3d::platform
