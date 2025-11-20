#pragma once

#include <cstdint>

namespace rflect {

struct Resolution {
  std::uint16_t width {1920};
  std::uint16_t height {1080};

  bool operator==(Resolution const& other) const = default;
};

} // namespace rflect
