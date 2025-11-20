
#include "rflect3d/core/logging/logger_config.hpp"
#include <source_location>

#include <rflect3d/config/parser.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>

struct ConfigExample {
  rflect::config::Logger logger{};
};

inline bool check_file(std::filesystem::path const &path) {
  if (not std::filesystem::exists(path)) {
    return false;
  }
  return rflect::config::parse_file<ConfigExample>(path) ? true : false;
}

TEST_SUITE_BEGIN("Program");

TEST_CASE("Program configuration") {
  std::filesystem::path const current_file =
      std::source_location::current().file_name();
  std::filesystem::path const config_folder =
      current_file.parent_path() / "../../config";

  CHECK(check_file(config_folder / "default.toml"));
  CHECK_FALSE(check_file(config_folder / "bad_toml_syntax.toml"));
  CHECK_FALSE(check_file(config_folder / "bad_logger.toml"));
  CHECK_FALSE(check_file(config_folder / "missing_logger_name.toml"));
}

TEST_SUITE_END();
