#pragma once

#include <expected>
#include <stdexcept>

namespace mono::err {

template<typename T>
using expected = std::expected<T, std::runtime_error>;

inline std::unexpected<std::runtime_error> unexpected(std::string_view message) {
  return std::unexpected<std::runtime_error> {std::runtime_error(std::string(message))};
}

} // namespace mono::err
