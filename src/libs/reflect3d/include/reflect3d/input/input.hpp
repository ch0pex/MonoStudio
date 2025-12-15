#pragma once

#include <GLFW/glfw3.h>

namespace rf3d::input {

inline void poll_events() { glfwPollEvents(); }

} // namespace rf3d::input
