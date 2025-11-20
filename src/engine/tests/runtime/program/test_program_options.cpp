
#include "rflect3d/core/logging/logger_config.hpp"
#include <rflect3d/program/options.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>
#include <fstream>

struct SampleConfig {
  rflect::config::Logger logger{};
};

static std::filesystem::path create_temp_toml(std::string const &content) {
  auto tmp = std::filesystem::temp_directory_path() / "test_config.toml";
  std::ofstream ofs(tmp);
  ofs << content;
  return tmp;
}

struct TempFile {
  std::filesystem::path path;
  ~TempFile() {
    if (!path.empty() && std::filesystem::exists(path)) {
      std::filesystem::remove(path);
    }
  }
};

TEST_CASE("parse_options devuelve error si no se pasa ningún argumento") {
  std::array<char *, 1> argv = {const_cast<char *>("program")};
  auto result = rflect::po::parse_options<SampleConfig>(argv);

  CHECK_FALSE(result.has_value());
  CHECK(std::string{result.error().what()}.find("TOML config") !=
        std::string::npos);
}

TEST_CASE("help_message contiene las opciones y el ejemplo TOML") {
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Show help message")(
      "config,c", po::value<std::string>(), "Path to configuration file");

  auto help = rflect::po::detail::help_message(desc);

  CHECK(help.find("Allowed options") != std::string::npos);
  CHECK(help.find("TOML config") != std::string::npos);
}

TEST_CASE("Valid TOML") {
  TempFile temp;
  temp.path = create_temp_toml(rfl::toml::write(SampleConfig{}));

  std::array<char *, 3> argv = {const_cast<char *>("program"),
                                const_cast<char *>("--config"),
                                const_cast<char *>(temp.path.c_str())

  };

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK(result.has_value());
  CHECK(result->logger.name == "rflect3d");
  CHECK(result->logger.path == "/tmp/");
}

TEST_CASE("Invalid TOML") {
  TempFile temp;
  temp.path = create_temp_toml("invalid_toml = { this_is_not_valid }");

  std::array<char *, 3> argv = {const_cast<char *>("program"),
                                const_cast<char *>("--config"),
                                const_cast<char *>(temp.path.c_str())};

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK_FALSE(result.has_value());
}

TEST_CASE("Invalid options") {
  std::array<char *, 2> argv = {const_cast<char *>("program"),
                                const_cast<char *>("--wrong-option")};

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK_FALSE(result.has_value());
  CHECK(std::string{result.error().what()}.find("Error parsing command line") !=
        std::string::npos);
}
