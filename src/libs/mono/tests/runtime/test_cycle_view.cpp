#include <algorithm>
#include <mono/ranges/cycle_view.hpp>

#include <algorithm>
#include <array>
#include <list>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("CycleView");

using mono::views::cycle;
using mono::views::CycleView;

TEST_CASE("CycleView repeats forward range indefinitely") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  CHECK_EQ(*it, 1);
  ++it;
  CHECK_EQ(*it, 2);
  ++it;
  CHECK_EQ(*it, 3);
  ++it;
  CHECK_EQ(*it, 1);
  ++it;
  CHECK_EQ(*it, 2);
  ++it;
  CHECK_EQ(*it, 3);
  ++it;
  CHECK_EQ(*it, 1);
  ++it;
}

TEST_CASE("CycleView works with std::array") {
  std::array<int, 3> arr = {10, 20, 30};
  auto cycled            = cycle(arr);

  auto it = cycled.begin();
  for (int i = 0; i < 10; ++i) {
    CHECK_EQ(*it, arr[i % 3]);
    ++it;
  }
}

TEST_CASE("CycleView works with empty range") {
  std::vector<int> vec = {};
  auto cycled          = vec | cycle;

  CHECK(cycled.begin() == std::default_sentinel);
}

TEST_CASE("CycleView with single element repeats that element") {
  std::vector<int> vec = {42};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  for (int i = 0; i < 5; ++i) {
    CHECK_EQ(*it, 42);
    ++it;
  }
}

TEST_CASE("CycleView post-increment works correctly") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  CHECK_EQ(*(it++), 1);
  CHECK_EQ(*(it++), 2);
  CHECK_EQ(*(it++), 3);
  CHECK_EQ(*(it++), 1);
}

TEST_CASE("CycleView iterator equality comparison") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it1 = cycled.begin();
  auto it2 = cycled.begin();

  CHECK(it1 == it2);

  ++it1;
  CHECK_FALSE(it1 == it2);

  ++it2;
  CHECK(it1 == it2);
}

TEST_CASE("CycleView with bidirectional range supports decrement") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  ++it;
  ++it;
  ++it; // At 1 (second cycle)
  ++it; // At 2 (second cycle)

  --it;
  CHECK_EQ(*it, 1);
  --it;
  CHECK_EQ(*it, 3);
  --it;
  CHECK_EQ(*it, 2);
  --it;
  CHECK_EQ(*it, 1);
}

TEST_CASE("CycleView post-decrement works correctly") {
  std::vector<int> vec = {5, 6, 7};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  ++it;
  ++it;
  ++it;
  ++it; // At 6 (second cycle)

  CHECK_EQ(*(it--), 6);
  CHECK_EQ(*(it--), 5);
  CHECK_EQ(*(it--), 7);
  CHECK_EQ(*it, 6);
}

TEST_CASE("CycleView with random access range supports operator+=") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  it += 5;
  CHECK_EQ(*it, 3);

  it += 4;
  CHECK_EQ(*it, 1);
}

TEST_CASE("CycleView with random access range supports operator-=") {
  std::vector<int> vec = {10, 20, 30};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  it += 10;

  it -= 3;
  CHECK_EQ(*it, 20);

  it -= 2;
  CHECK_EQ(*it, 30);
}

TEST_CASE("CycleView with random access range supports operator[]") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  CHECK_EQ(it[0], 1);
  CHECK_EQ(it[1], 2);
  CHECK_EQ(it[2], 3);
  CHECK_EQ(it[3], 1);
  CHECK_EQ(it[4], 2);
  CHECK_EQ(it[5], 3);
  CHECK_EQ(it[6], 1);
  CHECK_EQ(it[10], 2);
}

TEST_CASE("CycleView iterator addition with integer") {
  std::vector<int> vec = {7, 8, 9};
  auto cycled          = vec | cycle;

  auto it  = cycled.begin();
  auto it2 = it + 5;
  CHECK_EQ(*it2, 9);

  auto it3 = 7 + it;
  CHECK_EQ(*it3, 8);
}

TEST_CASE("CycleView iterator subtraction with integer") {
  std::vector<int> vec = {4, 5, 6};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  it += 10;

  auto it2 = it - 3;
  CHECK_EQ(*it2, 5);
}

TEST_CASE("CycleView iterator difference") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it1 = cycled.begin();
  auto it2 = it1 + 7;

  CHECK_EQ(it2 - it1, 7);
  CHECK_EQ(it1 - it2, -7);
}

TEST_CASE("CycleView iterator comparison operators") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it1 = cycled.begin();
  auto it2 = it1 + 5;

  CHECK(it1 < it2);
  CHECK_FALSE(it2 < it1);

  CHECK(it2 > it1);
  CHECK_FALSE(it1 > it2);

  CHECK(it1 <= it2);
  CHECK(it1 <= it1);

  CHECK(it2 >= it1);
  CHECK(it1 >= it1);
}

TEST_CASE("CycleView iterator three-way comparison") {
  std::vector<int> vec = {1, 2, 3};
  auto cycled          = vec | cycle;

  auto it1 = cycled.begin();
  auto it2 = it1 + 5;
  auto it3 = cycled.begin();

  CHECK((it1 <=> it2) < 0);
  CHECK((it2 <=> it1) > 0);
  CHECK((it1 <=> it3) == 0);
}

TEST_CASE("CycleView base() returns underlying view") {
  std::vector<int> vec = {1, 2, 3};
  auto view            = std::views::all(vec);
  CycleView cycled(view);

  auto base = cycled.base();
  CHECK(std::ranges::equal(base, vec));
}

TEST_CASE("CycleView works with std::list (bidirectional range)") {
  std::list<int> lst = {10, 20, 30};
  auto cycled        = cycle(lst);

  auto it = cycled.begin();
  CHECK_EQ(*it, 10);
  ++it;
  CHECK_EQ(*it, 20);
  ++it;
  CHECK_EQ(*it, 30);
  ++it;
  CHECK_EQ(*it, 10);
  ++it;

  --it;
  CHECK_EQ(*it, 10);
  --it;
  CHECK_EQ(*it, 30);
}

TEST_CASE("CycleView with string iterates over characters") {
  std::string str = "abc";
  auto cycled     = cycle(str);

  auto it = cycled.begin();
  CHECK_EQ(*it, 'a');
  ++it;
  CHECK_EQ(*it, 'b');
  ++it;
  CHECK_EQ(*it, 'c');
  ++it;
  CHECK_EQ(*it, 'a');
  ++it;
  CHECK_EQ(*it, 'b');
  ++it;
}

TEST_CASE("CycleView can be used with algorithms") {
  std::vector<int> vec = {5, 10, 15};
  auto cycled          = vec | cycle;

  auto it = cycled.begin();
  std::vector<int> result(9);
  std::ranges::generate(result, [&it]() { return *(it++); });

  std::vector<int> expected = {5, 10, 15, 5, 10, 15, 5, 10, 15};
  CHECK(result == expected);
}

TEST_CASE("CycleView with const vector") {
  std::vector<int> const vec = {100, 200};
  auto cycled                = vec | cycle;

  auto it = cycled.begin();
  for (int i = 0; i < 6; ++i) {
    CHECK_EQ(*it, vec[i % 2]);
    ++it;
  }
}

TEST_CASE("CycleView iter_move works correctly") {
  std::vector<std::string> vec = {"hello", "world"};
  auto cycled                  = vec | cycle;

  auto it = cycled.begin();

  CHECK(std::is_same_v<decltype(std::ranges::iter_move(it)), std::string&&>);
}

TEST_CASE("CycleView deduction guide works") {
  std::vector<int> vec = {1, 2, 3};
  CycleView cv(std::views::all(vec));

  auto it = cv.begin();
  CHECK_EQ(*it, 1);
  ++it;
  CHECK_EQ(*it, 2);
}

TEST_SUITE_END();
