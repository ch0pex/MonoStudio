/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file logger.hpp
 * @version 1.0
 * @date 08/03/2024
 * @brief Logger
 *
 * Logger class
 */

#pragma once

#include "mono/logging/logger_config.hpp"

#include <quill/LogMacros.h>
#include <quill/Logger.h>

// STD
#include <print>

namespace mono {

using LogLevel = quill::LogLevel;

class GlobalLogger {
  GlobalLogger()  = default;
  ~GlobalLogger() = default;

public:
  GlobalLogger(GlobalLogger&&)                 = delete;
  GlobalLogger& operator=(GlobalLogger&&)      = delete;
  explicit GlobalLogger(GlobalLogger const&)   = delete;
  GlobalLogger& operator=(GlobalLogger const&) = delete;

  static quill::Logger* instance() { return instance_impl(); }

  static void configure(config::Logger const& config) { instance_impl() = config::create_logger(config); }

private:
  static quill::Logger*& instance_impl() noexcept try {
    static auto* logger = config::create_logger({.name = "Rflect", .path = std::filesystem::temp_directory_path()});
    return logger;
  }
  catch (std::exception& e) {
    std::println("Something went wrong while accessing to the logger. {}", e.what());
    std::exit(-1);
  }
};

// clang-format off
#define LOG_DYNAMIC(level, fmt, ...)      QUILL_LOG_DYNAMIC(mono::GlobalLogger::instance(), level, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)                QUILL_LOG_INFO(mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...)             QUILL_LOG_WARNING(mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...)               QUILL_LOG_ERROR(mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_INFO_LIMIT(min,fmt, ...)      QUILL_LOG_INFO_LIMIT(min, mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_WARNING_LIMIT(min,fmt, ...)   QUILL_LOG_WARNING_LIMIT(min, mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_ERROR_LIMIT(min, fmt, ...)    QUILL_LOG_ERROR_LIMIT(min, mono::GlobalLogger::instance(), fmt, ##__VA_ARGS__);

// clang-format on

} // namespace mono
