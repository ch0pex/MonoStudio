
#include "mono/logging/logger.hpp"

//
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>

// STD
#include <print>

namespace mono {

namespace {

quill::Logger*& get_global_logger() noexcept try {
  static auto* logger = create_logger({.name = "Rflect", .path = std::filesystem::temp_directory_path()});
  return logger;
}
catch (std::exception& e) {
  std::println("Something went wrong while accessing to the logger. {}", e.what());
  std::exit(-1);
}

} // namespace

quill::Logger* create_logger(LoggerConfig const& config) {
  if (not quill::Backend::is_running()) {
    quill::Backend::start();
  }
  auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(std::format("{}_console", config.name));
  auto file_sink    = quill::Frontend::create_or_get_sink<quill::FileSink>(
      std::format("{}/.log", config.path.string()),
      [] {
        quill::FileSinkConfig cfg;
        cfg.set_open_mode('w');
        cfg.set_filename_append_option(quill::FilenameAppendOption::StartDateTime);
        return cfg;
      }(),
      quill::FileEventNotifier {}
  );
  return quill::Frontend::create_or_get_logger(
      std::string {config.name}, {std::move(file_sink), std::move(console_sink)}
  );
}

void log_dynamic(LogLevel level, std::string_view message) { //
  QUILL_LOG_DYNAMIC(get_global_logger(), static_cast<quill::LogLevel>(level), "{}", message);
}

void log_info(std::string_view message) { //
  QUILL_LOG_INFO(get_global_logger(), "{}", message);
}

void log_warning(std::string_view message) { //
  QUILL_LOG_WARNING(get_global_logger(), "{}", message);
}

void log_error(std::string_view message) { //
  QUILL_LOG_ERROR(get_global_logger(), "{}", message);
}

void log_info_limit(std::uint64_t min_interval_ms, std::string_view message) {
  QUILL_LOG_INFO_LIMIT(std::chrono::milliseconds(min_interval_ms), get_global_logger(), "{}", message);
}

void log_warning_limit(std::uint64_t min_interval_ms, std::string_view message) {
  QUILL_LOG_WARNING_LIMIT(std::chrono::milliseconds(min_interval_ms), get_global_logger(), "{}", message);
}

void log_error_limit(std::uint64_t min_interval_ms, std::string_view message) {
  QUILL_LOG_ERROR_LIMIT(std::chrono::milliseconds(min_interval_ms), get_global_logger(), "{}", message);
}

} // namespace mono
