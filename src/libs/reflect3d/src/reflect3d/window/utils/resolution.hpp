#pragma once

#include <cstdint>

namespace rf3d {

struct Resolution {
  std::uint16_t width {1920};
  std::uint16_t height {1080};

  bool operator==(Resolution const& other) const = default;
};


} // namespace rf3d
