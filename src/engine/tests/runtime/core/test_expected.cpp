#include <rflect3d/core/error/expected.hpp>

#include <expected>
#include <stdexcept>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Error");

using rflect::err::expected;
using rflect::err::unexpected;

TEST_CASE("unexpected() without format arguments returns plain message") {
  auto err = unexpected("simple error");

  // Comprueba que el tipo sea std::unexpected<std::runtime_error>
  static_assert(
      std::is_same_v<decltype(err), std::unexpected<std::runtime_error>>);

  CHECK_THROWS_AS(std::rethrow_exception(std::make_exception_ptr(err.error())),
                  std::runtime_error);

  CHECK_EQ(std::string(err.error().what()), "simple error");
}

TEST_CASE("unexpected() with format arguments substitutes values correctly") {
  auto err = unexpected("error {} occurred at {}", 404, "main.cpp");

  CHECK_EQ(std::string(err.error().what()), "error 404 occurred at main.cpp");
}

TEST_CASE("expected<T> works correctly with success case") {
  expected<int> result = 42;
  CHECK(result.has_value());
  CHECK_EQ(result.value(), 42);
}

TEST_CASE("expected<T> works correctly with unexpected error") {
  auto err = unexpected("boom");
  expected<int> result = err;

  CHECK_FALSE(result.has_value());
  CHECK_THROWS_AS(result.value(), std::bad_expected_access<std::runtime_error>);
  CHECK_EQ(std::string(result.error().what()), "boom");
}

TEST_SUITE_END();
