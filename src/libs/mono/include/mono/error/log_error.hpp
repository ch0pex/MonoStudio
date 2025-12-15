#pragma once

#include "mono/error/expected.hpp"

#include <print>
#include <stdexcept>

namespace mono::err {

template<typename T>
inline constexpr auto log = [](std::runtime_error const& error) -> mono::err::expected<T> {
  std::println("\033[31m{}\033[0m", error.what()); // 31 = red, 0 = reset
  return mono::err::unexpected(error.what());
};

} // namespace mono::err
