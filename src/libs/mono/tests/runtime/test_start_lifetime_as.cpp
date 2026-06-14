#include <bit>
#include <mono/misc/start_lifetime_as.hpp>

#include <array>
#include <complex>
#include <mono/containers/span.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


namespace {

TEST_CASE("Start lifetime as array") {
  alignas(std::complex<float>) std::array<unsigned char, sizeof(std::complex<float>)> network_data {
    0xcd, 0xcc, 0xcc, 0x3d, 0xcd, 0xcc, 0x4c, 0x3e,
  };

  auto* d2 = mono::start_lifetime_as<std::complex<float>>(network_data.data());

  std::complex<float> expected {0.1F, 0.2F};
  CHECK(*d2 == expected);
}

TEST_CASE("Start lifetime as array multiple") {
  alignas(std::complex<float>) std::array<unsigned char, sizeof(std::complex<float>) * 2> network_data {
    0xcd, 0xcc, 0xcc, 0x3d, 0xcd, 0xcc, 0x4c, 0x3e, //
    0x9a, 0x99, 0x19, 0x40, 0x9a, 0x99, 0x59, 0x40,
  };

  auto* d2 = mono::start_lifetime_as_array<std::complex<float>>(network_data.data(), 2);
  mono::span<std::complex<float>> const d2_span {d2, 2};

  std::complex<float> expected1 {0.1F, 0.2F};
  std::complex<float> expected2 {2.4F, 3.4F};

  CHECK(d2_span[0] == expected1);
  CHECK(d2_span[1] == expected2);
}

// --- Tests adapted from GCC's libstdc++ testsuite ---
struct S {
  int a;
  int b;
};
struct T {
  long long c;
};

static_assert(sizeof(S) == sizeof(T));

// NOLINTBEGIN
TEST_CASE("start_lifetime_as reinterprets memory between same-size types") {
  union U {
    unsigned char a[sizeof(S)];
    S b;
    T c;
  } u {};
  T const v = std::bit_cast<T>(S {.a = 1, .b = 2});

  S* d = mono::start_lifetime_as<S>(reinterpret_cast<void*>(&u.a));
  d->a = 1;
  d->b = 2;

  T* e = mono::start_lifetime_as<T>(reinterpret_cast<void*>(d));
  CHECK(e->c == v.c);

  T const* f = mono::start_lifetime_as<T>(reinterpret_cast<void const*>(d));
  CHECK(f->c == v.c);
}

TEST_CASE("start_lifetime_as_array reinterprets memory between same-size types") {
  union V {
    unsigned char a[3 * sizeof(S)];
    S b[3];
    T c[3];
  } w {};
  T const v = std::bit_cast<T>(S {.a = 1, .b = 2});
  T const x = std::bit_cast<T>(S {.a = 3, .b = 4});
  T const y = std::bit_cast<T>(S {.a = 5, .b = 6});

  S* i   = mono::start_lifetime_as_array<S>(reinterpret_cast<void*>(&w.a), 3);
  i[0].a = 1;
  i[0].b = 2;
  i[1].a = 3;
  i[1].b = 4;
  i[2].a = 5;
  i[2].b = 6;

  T* j = mono::start_lifetime_as_array<T>(reinterpret_cast<void*>(i), 3);
  CHECK(j[0].c == v.c);
  CHECK(j[1].c == x.c);
  CHECK(j[2].c == y.c);

  T const* k = mono::start_lifetime_as_array<T>(reinterpret_cast<void const*>(i), 3);
  CHECK(k[0].c == v.c);
  CHECK(k[1].c == x.c);
  CHECK(k[2].c == y.c);
}

TEST_CASE("start_lifetime_as_array with null pointer returns nullptr") {
  CHECK(mono::start_lifetime_as_array<T>(static_cast<void*>(nullptr), 0) == nullptr);
  CHECK(mono::start_lifetime_as_array<T>(static_cast<void const*>(nullptr), 0) == nullptr);
}

TEST_CASE("start_lifetime_as on const static array") {
  T const x          = std::bit_cast<T>(S {.a = 3, .b = 4});
  T const y          = std::bit_cast<T>(S {.a = 5, .b = 6});
  static S const r[] = {{.a = 5, .b = 6}, {.a = 3, .b = 4}};

  T const* s = mono::start_lifetime_as<T>(&r[1]);
  CHECK(s->c == x.c);

  T const* t = mono::start_lifetime_as_array<T>(&r[0], 2);
  CHECK(t[0].c == y.c);
  CHECK(t[1].c == x.c);
}

} // namespace

// NOLINTEND
