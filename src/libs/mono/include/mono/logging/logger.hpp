#pragma once

#include "mono/logging/logger_config.hpp"

#include <quill/Frontend.h>
#include <quill/LogMacros.h>

#include <cstdint>
#include <print>

namespace mono {

enum class LogLevel : uint8_t {
  trace_l3,
  trace_l2,
  trace_l1,
  debug,
  info,
  notice,
  warning,
  error,
  critical,
  backtrace, /**< This is only used for backtrace logging. Should not be set by the user. */
  none
};

inline quill::Logger*& get_global_logger() noexcept try {
  static auto* logger = create_logger({.name = "Rflect", .path = std::filesystem::temp_directory_path()});
  return logger;
}
catch (std::exception& e) {
  std::println("Something went wrong while accessing to the logger. {}", e.what());
  std::exit(-1);
}

// clang-format off
#define LOG_DYNAMIC(level, fmt, ...)      QUILL_LOG_DYNAMIC(mono::get_global_logger(), static_cast<quill::LogLevel>(level), fmt, ## __VA_ARGS__)
#define LOG_INFO(fmt, ...)                QUILL_LOG_INFO(mono::get_global_logger(), fmt, ## __VA_ARGS__)
#define LOG_WARNING(fmt, ...)             QUILL_LOG_WARNING(mono::get_global_logger(), fmt, ## __VA_ARGS__)
#define LOG_ERROR(fmt, ...)               QUILL_LOG_ERROR(mono::get_global_logger(), fmt, ## __VA_ARGS__)
#define LOG_INFO_LIMIT(min,fmt, ...)      QUILL_LOG_INFO_LIMIT(min, mono::get_global_logger(), fmt, ## __VA_ARGS__)
#define LOG_WARNING_LIMIT(min,fmt, ...)   QUILL_LOG_WARNING_LIMIT(min, mono::get_global_logger(), fmt, ## __VA_ARGS__)
#define LOG_ERROR_LIMIT(min, fmt, ...)    QUILL_LOG_ERROR_LIMIT(min, mono::get_global_logger(), fmt, ## __VA_ARGS__)
// clang-format on

} // namespace mono
