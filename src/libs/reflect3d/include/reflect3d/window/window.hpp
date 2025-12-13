#pragma once

#include "reflect3d/window/window_handle.hpp"

#include <GLFW/glfw3.h>


namespace rflect {

/**
 * @brief This window class destroys gflw window handle when destructor is invoked
 *
 * Can be used to controll the lifetime of a window handle
 * using RAII .
 *
 * As windows lifetime is actually controlled by
 * the user, this is only usefull to use with one window applications
 *
 * @note Use WindowHandles and WindowTracker class to build a multiple window application
 */
class Window : public WindowHandle {
public:
  // --- Type traits ---
  using handle_type = WindowHandle;

  // --- Constructors ---
  explicit Window(handle_type&& handle) : WindowHandle(std::move(handle)) { }

  explicit Window(Window&& other) noexcept : WindowHandle(std::move(other)) { }

  Window(Window const& other) = delete;

  Window& operator=(Window const& other) = delete;

  Window& operator=(Window&& other) noexcept = default;

  ~Window() { glfwDestroyWindow(native_handle()); }
};

} // namespace rflect
