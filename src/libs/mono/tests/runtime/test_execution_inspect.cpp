
#include <mono/execution/execution.hpp>

#include <print>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Execution");


int function(int const number) { return number + 1; }

TEST_CASE("Tap algorithm returned value ignored") {

  constexpr int initial_value = 42;

  auto sender = mono::ex::just(initial_value) //
                | mono::ex::inpsect(function) // Returned value will be ignored, tap only inspects
                | mono::ex::then(function);

  auto [value] = mono::ex::sync_wait(sender).value();

  DOCTEST_CHECK_EQ(value, initial_value + 1);
}

TEST_CASE("Tap algorithm inspecting") {
  constexpr int initial_value = 42;

  auto sender = mono::ex::just(initial_value) //
                | mono::ex::inpsect([](int const value) { std::println("Inspecting value {}", value); }) // Inspecting
                | mono::ex::then(function);

  auto [value] = mono::ex::sync_wait(sender).value();

  DOCTEST_CHECK_EQ(value, initial_value + 1);
}

TEST_SUITE_END();
