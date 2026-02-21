#pragma once

#include "mono/logging/logger_config.hpp"

#include <cstdint>
#include <string_view>

// Forward declaration de Quill para no incluir sus headers aquí

namespace mono {

enum class LogLevel : std::uint8_t { Debug = 0, Info, Error, Warning, Critical };

void log_dynamic(LogLevel level, std::string_view message);
void log_info(std::string_view message);
void log_warning(std::string_view message);
void log_error(std::string_view message);

void log_info_limit(std::uint64_t min_interval_ms, std::string_view message);
void log_warning_limit(std::uint64_t min_interval_ms, std::string_view message);
void log_error_limit(std::uint64_t min_interval_ms, std::string_view message);

// clang-format off
#define LOG_DYNAMIC(level, fmt, ...)  mono::log_dynamic(level, std::format(fmt, ##__VA_ARGS__))
#define LOG_INFO(fmt, ...)            mono::log_info(std::format(fmt, ##__VA_ARGS__))
#define LOG_WARNING(fmt, ...)         mono::log_warning(std::format(fmt, ##__VA_ARGS__))
#define LOG_ERROR(fmt, ...)           mono::log_error(std::format(fmt, ##__VA_ARGS__))

#define LOG_INFO_LIMIT(min, fmt, ...)    mono::log_info_limit(min, std::format(fmt, ##__VA_ARGS__))
#define LOG_WARNING_LIMIT(min, fmt, ...) mono::log_warning_limit(min, std::format(fmt, ##__VA_ARGS__))
#define LOG_ERROR_LIMIT(min, fmt, ...)   mono::log_error_limit(min, std::format(fmt, ##__VA_ARGS__))
// clang-format on

} // namespace mono
