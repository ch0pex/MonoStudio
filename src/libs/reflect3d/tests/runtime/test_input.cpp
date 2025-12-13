#include <reflect3d/input/input.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Input");

TEST_CASE("Poll events") { rflect::input::poll_events(); }

TEST_SUITE_END();
