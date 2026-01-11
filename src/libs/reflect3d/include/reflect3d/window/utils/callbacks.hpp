#pragma once

#include "mono/execution/stop_token.hpp"
#include "mono/logging/logger.hpp"

//
#include <GLFW/glfw3.h>

//
#ifdef MONO_USE_CPP_MODULES
#else
#include <chrono>
#include <span>
#include <string_view>
#endif

namespace rf3d::callbacks {

// Window event callback typedefs
using WindowClose        = GLFWwindowclosefun;
using WindowFocus        = GLFWwindowfocusfun;
using WindowRefresh      = GLFWwindowrefreshfun;
using WindowPos          = GLFWwindowposfun;
using WindowSize         = GLFWwindowsizefun;
using WindowIconify      = GLFWwindowiconifyfun;
using WindowMaximize     = GLFWwindowmaximizefun;
using FramebufferSize    = GLFWframebuffersizefun;
using WindowContentScale = GLFWwindowcontentscalefun;


// Input-related callback typedefs
using Key         = GLFWkeyfun;
using Char        = GLFWcharfun;
using CharMods    = GLFWcharmodsfun;
using MouseButton = GLFWmousebuttonfun;
using CursorPos   = GLFWcursorposfun;

using CursorEnter = GLFWcursorenterfun;

using Scroll = GLFWscrollfun;
using Drop   = GLFWdropfun;

namespace defaults {

// --- Helper to safely get window title as string_view ---
inline std::string_view get_title(GLFWwindow* handle) {
  char const* title = glfwGetWindowTitle(handle);
  // Determine length safely to satisfy the compiler optimizer
  return (title != nullptr) ? std::string_view(title) : std::string_view("Untitled");
}

// --- Window callbacks ---

inline constexpr WindowClose window_close = [](GLFWwindow* handle [[maybe_unused]]) {
  mono::ex::request_stop();
  LOG_INFO("Window '{}' is closing (close requested).", get_title(handle));
};

inline constexpr WindowFocus window_focus = [](GLFWwindow* handle [[maybe_unused]], int focus) {
  LOG_INFO("Window '{}' focus changed: {}", get_title(handle), std::string_view(focus ? "focused" : "unfocused"));
};

inline constexpr WindowRefresh window_refresh = [](GLFWwindow* handle [[maybe_unused]]) {
  LOG_INFO_LIMIT(std::chrono::seconds {1}, "Window '{}' requested refresh.", get_title(handle));
};

inline constexpr WindowPos window_pos = [](GLFWwindow* handle [[maybe_unused]], int x, int y) {
  LOG_INFO_LIMIT(std::chrono::seconds {1}, "Window '{}' moved to position ({}, {}).", get_title(handle), x, y);
};

inline constexpr WindowSize window_size = [](GLFWwindow* handle [[maybe_unused]], int width, int height) {
  LOG_INFO_LIMIT(std::chrono::seconds {1}, "Window '{}' resized to {}x{}.", get_title(handle), width, height);
};

inline constexpr WindowIconify window_iconify = [](GLFWwindow* handle [[maybe_unused]], int iconified) {
  LOG_INFO(
      "Window '{}' iconify state changed: {}", get_title(handle), std::string_view(iconified ? "minimized" : "restored")
  );
};

inline constexpr WindowMaximize window_maximize = [](GLFWwindow* handle [[maybe_unused]], int maximized) {
  LOG_INFO(
      "Window '{}' maximize state changed: {}", get_title(handle),
      std::string_view(maximized ? "maximized" : "restored")
  );
};

inline constexpr FramebufferSize framebuffer_size = [](GLFWwindow* handle [[maybe_unused]], int width, int height) {
  LOG_INFO_LIMIT(
      std::chrono::seconds {1}, "Framebuffer for window '{}' resized to {}x{}.", get_title(handle), width, height
  );
};

inline constexpr WindowContentScale window_content_scale = [](GLFWwindow* handle [[maybe_unused]], float xscale,
                                                              float yscale) {
  LOG_INFO_LIMIT(
      std::chrono::milliseconds {500}, "Content scale for window '{}' changed: x = {}, y = {}.", get_title(handle),
      xscale, yscale
  );
};

// --- Input callbacks ---

inline constexpr Key key = [](GLFWwindow* handle [[maybe_unused]], int key, int scancode, int action, int mods) {
  LOG_INFO(
      "Key event on window '{}': key = {}, scancode = {}, action = {}, mods = {}", get_title(handle), key, scancode,
      action, mods
  );
};

inline constexpr Char character = [](GLFWwindow* handle [[maybe_unused]], unsigned int codepoint) {
  LOG_INFO("Character input on window '{}': codepoint = {}", get_title(handle), codepoint);
};

inline constexpr CharMods char_mods = [](GLFWwindow* handle [[maybe_unused]], unsigned int codepoint, int mods) {
  LOG_INFO("Character with mods on window '{}': codepoint = {}, mods = {}", get_title(handle), codepoint, mods);
};

inline constexpr MouseButton mouse_button = [](GLFWwindow* handle [[maybe_unused]], int button, int action, int mods) {
  LOG_INFO("Mouse button on window '{}': button = {}, action = {}, mods = {}", get_title(handle), button, action, mods);
};

inline constexpr CursorPos cursor_pos = [](GLFWwindow* handle [[maybe_unused]], double xpos, double ypos) {
  LOG_INFO_LIMIT(
      std::chrono::seconds {1}, "Cursor moved on window '{}': position = ({}, {}).", get_title(handle), xpos, ypos
  );
};


inline constexpr CursorEnter cursor_enter = [](GLFWwindow* handle [[maybe_unused]], int entered) {
  // Wrap the ternary result in string_view to prevent "source size" errors
  LOG_INFO("Cursor {} window '{}'.", std::string_view(entered ? "entered" : "left"), get_title(handle));
};

inline constexpr Scroll scroll = [](GLFWwindow* handle [[maybe_unused]], double xoffset, double yoffset) {
  LOG_INFO_LIMIT(
      std::chrono::seconds {1}, "Scroll event on window '{}': offset = ({}, {}).", get_title(handle), xoffset, yoffset
  );
};

inline constexpr Drop drop = [](GLFWwindow* handle [[maybe_unused]], int count, char const** paths) {
  std::span<char const* const> path_span {paths, static_cast<std::size_t>(count)};
  LOG_INFO("File drop event on window '{}': {} file(s) dropped.", get_title(handle), count);
  for (int i = 0; i < count; ++i)
    LOG_INFO(" - {}", std::string_view(path_span[i]));
};

} // namespace defaults

} // namespace rf3d::callbacks
