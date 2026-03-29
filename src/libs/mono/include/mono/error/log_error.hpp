#pragma once

#include "mono/error/expected.hpp"

#include <print>
#include <stdexcept>

namespace mono {

template<typename T>
inline constexpr auto print_error = [](std::runtime_error const& error) -> mono::expected<T> {
  std::println("\033[31m{}\033[0m", error.what()); // 31 = red, 0 = reset
  return mono::unexpected(error.what());
};

} // namespace mono
