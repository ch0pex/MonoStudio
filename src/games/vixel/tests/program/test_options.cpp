

#include <vixel/program/options.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TEST_SUITE_BEGIN("Program");

TEST_CASE("No Program options, using default") {
  std::string program       = "program";
  std::array<char*, 1> argv = {program.data()};

  CHECK(vix::po::parse_options(argv));
}

TEST_CASE("Config") {
  // std::filesystem::path const current_file = std::source_location::current().file_name();
  // std::filesystem::path const config_folder =
  //     current_file.parent_path().parent_path() / "config" / std::format("default_{}.toml", rflect::os::name);
  // std::string config_path = config_folder.string();
  //
  // std::string program {"program"};
  // std::string config {"--config"};
  //
  // std::array<char*, 3> argv = {program.data(), config.data(), config_path.data()};
  //
  // CHECK(vix::po::parse_options(argv));
}

TEST_SUITE_END();
