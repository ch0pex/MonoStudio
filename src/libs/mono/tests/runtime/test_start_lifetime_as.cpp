
#include <mono/misc/start_lifetime_as.hpp>

#include <array>
#include <complex>
#include <span>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

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
  std::span<std::complex<float>> d2_span {d2, 2};

  std::complex<float> expected1 {0.1F, 0.2F};
  std::complex<float> expected2 {2.4F, 3.4F};

  CHECK(d2_span[0] == expected1);
  CHECK(d2_span[1] == expected2);
}
