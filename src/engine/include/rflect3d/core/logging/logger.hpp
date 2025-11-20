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

#include "rflect3d/core/logging/logger_config.hpp"
#include <quill/Logger.h>

namespace rflect {

class GlobalLogger {
  GlobalLogger() = default;
  ~GlobalLogger() = default;

public:
  explicit GlobalLogger(GlobalLogger const &) = delete;
  GlobalLogger &operator=(GlobalLogger const &) = delete;

  static quill::Logger *instance() { return logger; }

  static void configure(config::Logger const &config) {
    logger = config::create_logger(config);
  }

private:
  inline static quill::Logger *logger =
      config::create_logger({"Rflect", "/tmp/"});
};

// clang-format off

#define LOG_INFO(fmt, ...)                QUILL_LOG_INFO(rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...)             QUILL_LOG_WARNING(rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...)               QUILL_LOG_ERROR(rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_INFO_LIMIT(min,fmt, ...)      QUILL_LOG_INFO_LIMIT(min, rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_WARNING_LIMIT(min,fmt, ...)   QUILL_LOG_WARNING_LIMIT(min, rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);
#define LOG_ERROR_LIMIT(min, fmt, ...)    QUILL_LOG_ERROR_LIMIT(min, rflect::GlobalLogger::instance(), fmt, ##__VA_ARGS__);

// clang-format on

} // namespace rflect
