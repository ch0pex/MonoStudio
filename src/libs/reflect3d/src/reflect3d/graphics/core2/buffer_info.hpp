#pragma once

#include <mono/containers/span.hpp>

#include <cstdint>
#include <span>
#include <string>

namespace rf3d {

enum class BufferUsage : std::uint8_t {
  source      = (1 << 0),
  destination = (1 << 1),
  uniform     = (1 << 2),
  storage     = (1 << 3),
  vertex      = (1 << 4),
  index       = (1 << 5),
};

enum class MemoryProperty : std::uint8_t {
  dedicated = (1 << 0),
  mapped    = (1 << 1),
};

template<typename T>
  requires std::is_trivially_copyable_v<T>
struct BufferInfo {
  std::string name {"Unnamed Buffer"};
  mono::span<T const> data;
  std::size_t capacity = 0;
};

constexpr BufferUsage operator|(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<BufferUsage>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

} // namespace rf3d
