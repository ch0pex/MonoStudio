#pragma once

#include <GLFW/glfw3.h>

namespace rflect::input {

inline void poll_events() { glfwPollEvents(); }

} // namespace rflect::input
