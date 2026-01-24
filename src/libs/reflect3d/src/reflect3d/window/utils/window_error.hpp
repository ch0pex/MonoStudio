#pragma once

#include <GLFW/glfw3.h>

//
#include <stdexcept>

namespace rf3d {

struct WindowException : public std::runtime_error {
  explicit WindowException(std::string const& message) : std::runtime_error(message) { }
};

/**
 * Retrieves the last window error from GLFW
 *
 * @return last error description
 */
inline std::string get_window_error() {
  char const* error_description = nullptr;
  if (glfwGetError(&error_description) != GLFW_NO_ERROR) {
    return error_description != nullptr ? std::string(error_description) : "Unknown error";
  }
  return "No error";
}

// TOOD error wrapper >> check::error


} // namespace rf3d
