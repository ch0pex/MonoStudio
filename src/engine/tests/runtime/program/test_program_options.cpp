
#include <rflect3d/program/options.hpp>
#include "rflect3d/config/base_config.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>
#include <fstream>

struct SampleConfig {
  rflect::config::Engine engine;
};

static std::filesystem::path create_temp_toml(std::string const& content) {
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
  std::string program {"program"};
  std::string config {"--config"};
  std::array<char*, 1> argv = {program.data()};
  auto result               = rflect::po::parse_options<SampleConfig>(argv);

  CHECK_FALSE(result.has_value());
  CHECK(std::string {result.error().what()}.find("TOML config") != std::string::npos);
}

TEST_CASE("help_message contiene las opciones y el ejemplo TOML") {
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Show help message")("config,c", po::value<std::string>(), "Path to configuration file");

  auto help = rflect::po::detail::help_message<SampleConfig>(desc);

  CHECK(help.find("Allowed options") != std::string::npos);
  CHECK(help.find("TOML config") != std::string::npos);
}

TEST_CASE("Valid TOML") {
  std::string program {"program"};
  std::string config {"--config"};
  TempFile temp {};
  temp.path = create_temp_toml(rfl::toml::write(SampleConfig {}));
  auto arg  = temp.path.string();

  std::array<char*, 3> argv = {program.data(), config.data(), arg.data()};

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK(result.has_value());
  CHECK(result->engine.logger.name == "rflect3d");
  CHECK(result->engine.logger.path == std::filesystem::temp_directory_path());
}

TEST_CASE("Invalid TOML") {
  std::string program {"program"};
  std::string config {"--config"};

  TempFile temp;
  temp.path = create_temp_toml("invalid_toml = { this_is_not_valid }");

  std::string temp_path = temp.path.string();

  std::array<char*, 3> argv = {program.data(), config.data(), temp_path.data()};

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK_FALSE(result.has_value());
}

TEST_CASE("Invalid options") {
  std::string program {"program"};
  std::string config {"--wrong-config"};
  std::array<char*, 2> argv = {program.data(), config.data()};

  auto result = rflect::po::parse_options<SampleConfig>(argv);
  CHECK_FALSE(result.has_value());
  CHECK(std::string {result.error().what()}.find("Error parsing command line") != std::string::npos);
}
