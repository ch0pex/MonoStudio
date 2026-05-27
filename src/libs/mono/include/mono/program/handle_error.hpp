/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file handle_error.hpp
 * @date 26/05/2026
 * @brief Top-level program error handler for execution pipelines
 */

#pragma once

// --- Includes ---
#include "mono/execution/execution.hpp"
#include "mono/execution/signals.hpp"
#include "mono/logging/logger.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <exception>
#include <utility>

// --- System ---


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
