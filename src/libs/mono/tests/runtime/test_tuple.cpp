#include <mono/containers/tuple.hpp>

#include <string>
#include <tuple>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Tuple");

TEST_CASE("for_each iterates tuple elements in order") {
  auto const values = std::make_tuple(1, 2, 3, 4);

  std::vector<int> visited;
  mono::tuple::for_each(values, [&visited](auto value) { visited.push_back(value); });

  CHECK(visited == std::vector<int> {1, 2, 3, 4});
}

TEST_CASE("for_each forwards mutable references") {
  auto values = std::make_tuple(1, 2, 3);

  mono::tuple::for_each(values, [](auto& value) { value *= 2; });

  CHECK_EQ(std::get<0>(values), 2);
  CHECK_EQ(std::get<1>(values), 4);
  CHECK_EQ(std::get<2>(values), 6);
}

TEST_CASE("transform maps every tuple element and returns tuple") {
  auto const values = std::make_tuple(1, 2, 3);

  auto const transformed = mono::tuple::transform(values, [](auto value) -> int { return value + 10; });

  static_assert(std::is_same_v<std::decay_t<decltype(transformed)>, std::tuple<int, int, int>>);
  CHECK_EQ(std::get<0>(transformed), 11);
  CHECK_EQ(std::get<1>(transformed), 12);
  CHECK_EQ(std::get<2>(transformed), 13);
}

TEST_CASE("transform supports heterogenous tuples") {
  auto values = std::make_tuple(5, std::string {"mono"});

  auto transformed = mono::tuple::transform(std::move(values), [](auto&& value) {
    using value_t = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<value_t, int>) {
      return value * 3;
    }
    else {
      return std::forward<decltype(value)>(value) + "_studio";
    }
  });

  static_assert(std::is_same_v<decltype(transformed), std::tuple<int, std::string>>);
  CHECK_EQ(std::get<0>(transformed), 15);
  CHECK_EQ(std::get<1>(transformed), "mono_studio");
}

TEST_SUITE_END();


namespace { }
