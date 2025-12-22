
#include <mono/execution/execution.hpp>
#include <mono/execution/expect.hpp>

#include <exception>
#include <print>
#include "mono/error/expected.hpp"


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Execution");


mono::err::expected<int> expected_function(int const number) {
  if (number == 42) {
    return number + 1;
  }
  return mono::err::unexpected("There was an error");
}

std::optional<int> optional_function(int const number) {
  if (number == 42) {
    return number + 1;
  }
  return std::nullopt;
}

mono::ex::sender auto handle_error(std::exception_ptr const& err [[maybe_unused]]) { return mono::ex::just(0); }

template<typename F>
int test_function(F func, int const number) {
  auto sender = mono::ex::just(number) //
                | mono::ex::expect(std::move(func)) //
                | mono::ex::let_error(handle_error); //

  auto [value] = mono::ex::sync_wait(sender).value();
  return value;
}


TEST_CASE("Expect algorithm with std::expected") {
  CHECK_EQ(test_function(expected_function, 42), 43);
  CHECK_EQ(test_function(expected_function, 40), 0);
}

TEST_CASE("Expect algorithm with std::optional") {
  CHECK_EQ(test_function(optional_function, 42), 43);
  CHECK_EQ(test_function(optional_function, 40), 0);
}


TEST_SUITE_END();
