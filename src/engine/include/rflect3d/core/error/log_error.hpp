#pragma once

#include "rflect3d/core/error/expected.hpp"

#include <print>
#include <stdexcept>

namespace rflect::err {

template<typename T>
inline auto log = [](std::runtime_error const& error) -> rflect::err::expected<T> {
  std::println("\033[31m{}\033[0m", error.what()); // 31 = red, 0 = reset
  return rflect::err::unexpected(error.what());
};

} // namespace rflect::err
