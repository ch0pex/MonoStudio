#include <mono/misc/as_span.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

namespace {


TEST_CASE("as_span for one element") {
  std::uint32_t const my_only_number = 42;

  auto view = mono::as_span(my_only_number);

  CHECK_EQ(view.size(), 1);
  CHECK_EQ(view[0], 42);
}

TEST_CASE("as_span for one element with write access") {
  std::uint32_t my_only_number = 42;

  auto view = mono::as_writeable_span(my_only_number);

  CHECK_EQ(view.size(), 1);
  CHECK_EQ(view[0], 42);

  view[0] = 100;

  CHECK_EQ(my_only_number, 100);
}

} // namespace
