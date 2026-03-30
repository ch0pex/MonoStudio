/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
#include "mono/misc/reference.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

void test_safe_ptr_covertion(int* pointer) { *pointer = 10; }

TEST_CASE("SafePtr constructors and dereference") {
  int value = 42;

  mono::SafePtr<int> ptr(value);
  mono::SafePtr<int> deduced_ptr = mono::SafePtr(value);
  mono::SafePtr<int const> const_ptr(value);

  CHECK_EQ(*ptr, 42);
  CHECK_EQ(*deduced_ptr, 42);
  CHECK_EQ(*const_ptr, 42);
}

TEST_CASE("SafePtr copy and move semantics") {
  int value = 5;
  mono::SafePtr ptr1 {value};

  test_safe_ptr_covertion(ptr1);
  CHECK_EQ(value, 10);
}

TEST_CASE("SafePtr arrow operator and pointer conversion") {
  struct Counter {
    int value;
  };

  Counter counter {7};
  mono::SafePtr<Counter> ptr(counter);

  CHECK_EQ(ptr->value, 7);

  ptr->value = 11;
  CHECK_EQ(counter.value, 11);

  Counter* raw_ptr = ptr;
  CHECK_EQ(raw_ptr, &counter);
}

TEST_CASE("SafePtr supports implicit convertible references") {
  struct Base {
    virtual ~Base() = default;
    int value       = 5;
  };
  struct Derived : Base { };

  Derived derived {};
  mono::SafePtr<Base> base_ptr(derived);

  CHECK_EQ(base_ptr->value, 5);
  CHECK_EQ(static_cast<Base*>(base_ptr), static_cast<Base*>(&derived));
}

TEST_CASE("SafePtr const correctness") {
  int value = 3;
  mono::SafePtr<int> ptr(value);
  mono::SafePtr<int const> const_ptr(value);

  static_assert(std::is_same_v<decltype(*ptr), int&>);
  static_assert(std::is_same_v<decltype(*const_ptr), int const&>);
  static_assert(std::is_same_v<decltype(ptr.operator->()), int*>);
  static_assert(std::is_same_v<decltype(const_ptr.operator->()), int const*>);
  static_assert(std::is_same_v<decltype(static_cast<int*>(ptr)), int*>);
  static_assert(std::is_same_v<decltype(static_cast<int const*>(const_ptr)), int const*>);
}
