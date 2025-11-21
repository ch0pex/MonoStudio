

#include <vixel/program/options.hpp>
#include "vixel/program/config/config.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


TEST_SUITE_BEGIN("Program");

TEST_CASE("Program options") {
  std::array<char*, 1> argv = {const_cast<char*>("program")};

  CHECK(vix::po::parse_options(argv));
  CHECK_FALSE(rflect::po::parse_options<vix::config::Vixel>(argv));
}

TEST_SUITE_END();
