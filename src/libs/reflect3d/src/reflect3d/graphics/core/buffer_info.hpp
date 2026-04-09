#pragma once

#include <mono/containers/span.hpp>

#include <cstdint>
#include <string>

namespace rf3d {

enum class MemoryProperty : std::uint8_t {
  dedicated = (1 << 0),
  mapped    = (1 << 1),
};

enum class BufferUsage : std::uint8_t {
  source      = (1 << 0),
  destination = (1 << 1),
  uniform     = (1 << 2),
  storage     = (1 << 3),
  vertex      = (1 << 4),
  index       = (1 << 5),
};

constexpr BufferUsage operator|(BufferUsage lhs, BufferUsage rhs) {
  using T = std::underlying_type_t<BufferUsage>;
  return static_cast<BufferUsage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

constexpr BufferUsage operator&(BufferUsage lhs, BufferUsage rhs) {
  using T = std::underlying_type_t<BufferUsage>;
  return static_cast<BufferUsage>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

constexpr BufferUsage& operator|=(BufferUsage& lhs, BufferUsage rhs) {
  lhs = lhs | rhs;
  return lhs;
}

template<typename T>
  requires std::is_trivially_copyable_v<T>
struct BufferInfo {
  std::string name {"Unnamed Buffer"};
  mono::span<T const> data;
  std::size_t capacity = 0;
};

struct BufferCopyRegion {
  std::size_t src_offset = 0;
  std::size_t dst_offset = 0;
  std::size_t size       = 0;
};

} // namespace rf3d
