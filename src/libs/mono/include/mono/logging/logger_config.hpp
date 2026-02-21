#pragma once

#include <mono/config/base_config.hpp>
//
#include <filesystem>

namespace quill {

inline namespace v11 {

struct FrontendOptions;

template<typename TFrontendOptions>
class LoggerImpl;

using Logger = LoggerImpl<FrontendOptions>;
} // namespace v11

} // namespace quill

namespace mono {

struct LoggerConfig {
  std::string name {"mono"};
  std::filesystem::path path {std::filesystem::temp_directory_path()};
};

quill::Logger* create_logger(LoggerConfig const& config);

} // namespace mono
