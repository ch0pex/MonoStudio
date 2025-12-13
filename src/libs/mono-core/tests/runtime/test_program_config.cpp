
#include <source_location>

#include <mono-core/config/parser.hpp>
#include <mono-core/platform/platform.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>

struct ConfigExample {
  rflect::config::Engine engine;
};

inline bool check_file(std::filesystem::path const& path) {
  if (not std::filesystem::exists(path)) {
    return false;
  }
  auto result = rflect::config::parse_file<ConfigExample>(path);
  if (not result) {
    LOG_ERROR("{}", result.error().what());
    return false;
  }
  return true;
}

TEST_SUITE_BEGIN("Program");

TEST_CASE("Program configuration") {
  std::filesystem::path const current_file  = std::source_location::current().file_name();
  std::filesystem::path const config_folder = current_file.parent_path().parent_path() / "config";

  CHECK(check_file(config_folder / std::format("default_{}.toml", rflect::os::name)));
  CHECK_FALSE(check_file(config_folder / "bad_toml_syntax.toml"));
  CHECK_FALSE(check_file(config_folder / "bad_logger.toml"));
  CHECK_FALSE(check_file(config_folder / "missing_logger_name.toml"));
}

TEST_SUITE_END();
