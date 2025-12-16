#pragma once

#include "mono/execution/execution.hpp"
#include "mono/execution/signals.hpp"
#include "mono/logging/logger.hpp"

#include <exception>
#include <utility>

namespace mono::program {

inline ex::sender auto handle_error(std::exception_ptr err) {
  try {
    std::rethrow_exception(std::move(err));
  }
  catch (std::exception const& e) {
    LOG_ERROR("A fatal error ocurred during program execution: {}", e.what());
  }
  catch (...) {
    LOG_ERROR("Unkown error ocurred! Shutting down program.");
  }
  ex::request_stop();
  return ex::just();
}

} // namespace mono::program
