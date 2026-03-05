#pragma once

#include <cstdint>
#include <span>
#include <string>

namespace rf3d::gfx {

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
  std::string name;
  std::span<T const> data;
  std::size_t capacity = 0;
};

} // namespace rf3d::gfx
