

#include <vixel/program/options.hpp>
#include "vixel/program/config/config.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TEST_SUITE_BEGIN("Program");

TEST_CASE("No Program options") {
  std::string program       = "program";
  std::array<char*, 1> argv = {program.data()};

  CHECK_FALSE(vix::po::parse_options(argv));
  CHECK_FALSE(mono::program::parse_options<vix::config::Vixel>(argv));
}

TEST_SUITE_END();
