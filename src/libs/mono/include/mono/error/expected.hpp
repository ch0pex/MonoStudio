#pragma once

#include <expected>
#include <stdexcept>

namespace mono {

template<typename T, typename Error = std::runtime_error>
using expected = std::expected<T, std::runtime_error>;

inline std::unexpected<std::runtime_error> unexpected(std::string_view message) {
  return std::unexpected<std::runtime_error> {std::runtime_error(std::string(message))};
}

} // namespace mono
