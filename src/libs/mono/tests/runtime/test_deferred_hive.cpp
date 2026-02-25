#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <doctest/doctest.h>
#include <mono/containers/hive.hpp>
#include <numeric>
#include <ranges>
#include <vector>

TEST_SUITE("Containers - DeferredHive") {

  TEST_CASE("Multi-tick Jump") {
    constexpr std::uint8_t delay = 5;
    mono::DeferredHive<int, delay> hive;

    auto it = hive.insert(10);
    hive.erase(it);

    SUBCASE("Partial tick jump") {
      hive.tick(3);
      CHECK_FALSE(hive.empty());
      CHECK(hive.size() == 1);

      hive.tick(2); // Total 5
      CHECK(hive.empty());
    }

    SUBCASE("Full delay jump") {
      hive.tick(delay);
      CHECK(hive.empty());
    }

    SUBCASE("Overshooting delay") {
      hive.tick(delay + 10);
      CHECK(hive.empty());
    }
  }

  TEST_CASE("Iteration and Ranges") {
    mono::DeferredHive<int, 3> hive;
    std::vector<int> values = {1, 2, 3, 4, 5};

    for (int v: values) {
      hive.insert(v);
    }

    SUBCASE("Standard iterator traversal") {
      int sum = 0;
      for (int& it: hive) {
        sum += it;
      }
      CHECK(sum == 15);
    }

    SUBCASE("C++20 Range compatibility") {
      // Testing filter and transform on the hive
      auto view = hive | std::views::filter([](int n) { return n % 2 == 0; }) |
                  std::views::transform([](int n) { return n * 2; });

      std::vector<int> result;
      for (int v: view) {
        result.push_back(v);
      }

      // Should find 2 and 4, transformed to 4 and 8
      CHECK(result.size() == 2);
      CHECK(std::ranges::contains(result, 4));
      CHECK(std::ranges::contains(result, 8));
    }
  }

  TEST_CASE("Iteration during pending deletion") {
    mono::DeferredHive<int, 10> hive;
    hive.insert(10);
    auto it = hive.insert(20);
    hive.insert(30);

    hive.erase(it); // 20 is marked

    // Even though 20 is marked, it must remain visible in iterations
    int count = 0;
    for (int val [[maybe_unused]]: hive) {
      count++;
    }
    CHECK(count == 3);

    hive.tick(10);

    count = 0;
    for (int val [[maybe_unused]]: hive) {
      count++;
    }
    CHECK(count == 2);
  }

  TEST_CASE("Complex Range Erasure and Ticking") {
    mono::DeferredHive<int, 2> hive;
    for (int i = 0; i < 10; ++i)
      hive.insert(i);

    // Erase first 5 elements
    auto it_mid = hive.begin();
    std::ranges::advance(it_mid, 5);
    hive.erase(hive.begin(), it_mid);

    CHECK(hive.size() == 10);

    hive.tick(1);
    CHECK(hive.size() == 10); // Not yet

    hive.tick(1);
    CHECK(hive.size() == 5); // Now deleted

    // Verify remaining elements [5, 6, 7, 8, 9]
    int min_val = 100;
    for (int v: hive)
      min_val = std::min(v, min_val);
    CHECK(min_val == 5);
  }
}
