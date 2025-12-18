#pragma once

#include <mono/config/base_config.hpp>

#define QUILL_DISABLE_NON_PREFIXED_MACROS
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>

//
#include <filesystem>

namespace mono::config {

struct Logger {
  std::string name {"reflect3d"};
  std::filesystem::path path {std::filesystem::temp_directory_path()};
};

inline quill::Logger* create_logger(Logger const& config) {
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

} // namespace mono::config
