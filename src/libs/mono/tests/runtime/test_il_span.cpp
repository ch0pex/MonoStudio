#include <numeric>
#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <mono/containers/span.hpp>

TEST_CASE("mono::span supports initializer-list construction for const element types") {
  static_assert(std::is_constructible_v<mono::span<int const>, std::initializer_list<int>>);
  static_assert(std::is_constructible_v<mono::span<int const, 3>, std::initializer_list<int>>);
  static_assert(!std::is_constructible_v<mono::span<int>, std::initializer_list<int>>);

  SUBCASE("direct construction from braces") {
    auto values = mono::span<int const> {1, 2, 3, 4, 5};

    CHECK_EQ(values.size(), 5);
    CHECK_FALSE(values.empty());
    CHECK_EQ(values.front(), 1);
    CHECK_EQ(values[2], 3);
    CHECK_EQ(values.back(), 5);
  }

  SUBCASE("fixed-extent construction from braces") {
    auto fixed = mono::span<int const, 3> {4, 5, 6};

    CHECK_EQ(fixed.size(), 3);
    CHECK_EQ(fixed[0], 4);
    CHECK_EQ(fixed[2], 6);
  }

  SUBCASE("empty initializer list creates an empty span") {
    auto empty = mono::span<int const> {};

    CHECK(empty.empty());
    CHECK_EQ(empty.size(), 0);
  }

  SUBCASE("passing initializer lists to functions taking span") {
    auto sum_values = [](mono::span<int const> data) { return std::accumulate(data.begin(), data.end(), 0); };

    CHECK_EQ(sum_values({10, 20, 30}), 60);
    CHECK_EQ(sum_values({}), 0);
  }
}

TEST_CASE("mono::span preserves pointer and pointee constness semantics") {
  SUBCASE("span of const pointers allows mutating pointed objects") {
    int a = 1;
    int b = 2;

    auto increment_all = [](mono::span<int* const> pointers) {
      for (int* pointer: pointers) {
        ++(*pointer);
      }
    };

    increment_all({&a, &b});

    CHECK_EQ(a, 2);
    CHECK_EQ(b, 3);
  }

  SUBCASE("span of pointers-to-const remains read-only for pointees") {
    int x        = 100;
    int const* p = &x;

    auto read_first = [](mono::span<int const* const> pointers) { return *pointers[0]; };

    CHECK_EQ(read_first({p}), 100);
  }
}
