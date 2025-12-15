#pragma once

#include <expected>
#include <format>
#include <stdexcept>

namespace mono::err {

template<typename T>
using expected = std::expected<T, std::runtime_error>;

template<typename... Args>
std::unexpected<std::runtime_error> unexpected(std::string_view const str, Args const&... args) {
  std::string message;
  if constexpr (sizeof...(Args) == 0) {
    message = str;
  }
  else {
    message = std::vformat(str, std::make_format_args(args...));
  }
  return std::unexpected<std::runtime_error> {std::runtime_error(message)};
}

} // namespace mono::err
