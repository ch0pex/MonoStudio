#pragma once

#include "rflect3d/core/execution/execution.hpp"
#include "rflect3d/core/execution/signals.hpp"
#include "rflect3d/core/logging/logger.hpp"

#include <exception>

namespace rflect::program {

inline ex::sender auto handle_error(std::exception_ptr err) {
  try {
    std::rethrow_exception(err);
  }
  catch (std::exception const& e) {
    LOG_ERROR("An exception was thrown: {}", e.what());
  }
  catch (...) {
    LOG_ERROR("Unkown error ocurred! Shutting down vixel.");
  }
  ex::request_stop();
  return ex::just();
}

} // namespace rflect::program
