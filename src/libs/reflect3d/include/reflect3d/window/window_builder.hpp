#pragma once

#include "reflect3d/window/utils/callbacks.hpp"
#include "reflect3d/window/utils/window_error.hpp"
#include "reflect3d/window/window.hpp"
#include "reflect3d/window/window_config.hpp"
#include "reflect3d/window/window_modes.hpp"

#include <mono/logging/logger.hpp>
#include <mono/misc/passkey.hpp>

#include <GLFW/glfw3.h>

namespace rf3d {

/**
 * @brief Builder class to create platform windows
 *
 * By providing a window configuration the user can create
 * plaform independent windows.
 *
 * It also facilatates setting the desired callbacks for callbacks
 * for such window.
 *
 * 'default_callbacks' method can be used to set default debugging callbacks
 */
class WindowBuilder {
public:
  using native_type = GLFWwindow*;

  explicit WindowBuilder(config::Window const& config = {}) {
    GlfwContext::init();

    // Handle must be created after GLFW context
    handle = detail::native_window(config); // NOLINT

    // TODO improve this error handling
    if (handle == nullptr) {
      std::string error_message = "Failed to create window: " + get_window_error();
      LOG_ERROR("{}", error_message);
      throw WindowException {error_message};
    }
  }
  // --- Window Surface ---

  template<typename Renderder>
  WindowBuilder& attach_surface(Renderder& renderer) const {
    renderer.create_surface(handle);
  }

  // --- Window Callbacks ---

  WindowBuilder& on_close(callbacks::WindowClose cb) {
    glfwSetWindowCloseCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_focus(callbacks::WindowFocus cb) {
    glfwSetWindowFocusCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_refresh(callbacks::WindowRefresh cb) {
    glfwSetWindowRefreshCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_position(callbacks::WindowPos cb) {
    glfwSetWindowPosCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_size(callbacks::WindowSize cb) {
    glfwSetWindowSizeCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_iconify(callbacks::WindowIconify cb) {
    glfwSetWindowIconifyCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_maximize(callbacks::WindowMaximize cb) {
    glfwSetWindowMaximizeCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_framebuffer_size(callbacks::FramebufferSize cb) {
    glfwSetFramebufferSizeCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_content_scale(callbacks::WindowContentScale cb) {
    glfwSetWindowContentScaleCallback(handle, cb);
    return *this;
  }

  // --- Input Callbacks ---

  WindowBuilder& on_key(callbacks::Key cb) {
    glfwSetKeyCallback(handle, cb);

    return *this;
  }

  WindowBuilder& on_char(callbacks::Char cb) {
    glfwSetCharCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_char_mods(callbacks::CharMods cb) {
    glfwSetCharModsCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_mouse_button(callbacks::MouseButton cb) {
    glfwSetMouseButtonCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_cursor_position(callbacks::CursorPos cb) {
    glfwSetCursorPosCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_cursor_enter(callbacks::CursorEnter cb) {
    glfwSetCursorEnterCallback(handle, cb);

    return *this;
  }

  WindowBuilder& on_scroll(callbacks::Scroll cb) {
    glfwSetScrollCallback(handle, cb);
    return *this;
  }

  WindowBuilder& on_drop(callbacks::Drop cb) {
    glfwSetDropCallback(handle, cb);
    return *this;
  }

  // Registra todas las callbackas por defecto
  WindowBuilder& default_callbacks() {
    using namespace callbacks::defaults;

    return this->on_close(window_close)
        .on_focus(window_focus)
        .on_refresh(window_refresh)
        .on_position(window_pos)
        .on_size(window_size)
        .on_iconify(window_iconify)
        .on_maximize(window_maximize)
        .on_framebuffer_size(framebuffer_size)
        .on_content_scale(window_content_scale)
        .on_key(key)
        .on_char(character)
        .on_char_mods(char_mods)
        .on_mouse_button(mouse_button)
        .on_cursor_position(cursor_pos)
        .on_cursor_enter(cursor_enter)
        .on_scroll(scroll)
        .on_drop(drop);
  }

  Window build() {
    Window window_handle {handle, mono::PassKey<WindowBuilder> {}};
    handle = nullptr;
    return window_handle;
  }

private:
  native_type handle {};
};

} // namespace rf3d
