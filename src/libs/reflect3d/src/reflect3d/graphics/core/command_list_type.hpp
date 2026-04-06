#pragma once

#include <cstdint>

namespace rf3d {

enum CommandListType : std::uint8_t {
  copy     = 1 << 0,
  compute  = 1 << 1,
  graphics = 1 << 2,
};

} // namespace rf3d
