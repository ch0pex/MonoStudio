#pragma once

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>

namespace mono {

struct LoggerConfig {
  std::string name {"mono"};
  std::filesystem::path path {std::filesystem::temp_directory_path()};
};

inline quill::Logger* create_logger(LoggerConfig const& config) {
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

} // namespace mono
