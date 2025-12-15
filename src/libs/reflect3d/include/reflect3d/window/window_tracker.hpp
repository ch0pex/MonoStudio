#pragma once


#include "reflect3d/window/window_handle.hpp"

//
#include <GLFW/glfw3.h>

//
#include <initializer_list>
#include <vector>

namespace rf3d {

/**
 * TODO Actually, this should be a tree structure
 * For now it just tracks multiple windows
 */
class WindowTracker {
public:
  using window_type = WindowHandle;

  WindowTracker() = default;

  explicit WindowTracker(std::initializer_list<window_type> windows) : windows(windows) { }

  void track(window_type handle) { windows.emplace_back(std::move(handle)); }

  [[nodiscard]] std::size_t count() const { return windows.size(); }

  [[nodiscard]] bool should_close() {
    for (auto it = windows.begin(); it != windows.end();) {
      if (it->should_close()) {
        glfwDestroyWindow(it->native_handle());
        it = windows.erase(it);
      }
      else {
        ++it;
      }
    }
    return windows.empty();
  }

private:
  std::vector<WindowHandle> windows;
};


} // namespace rf3d
