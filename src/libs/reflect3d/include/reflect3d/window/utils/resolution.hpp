#pragma once

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <cstdint>
#include <format>
#endif

namespace rf3d {

struct Resolution {
  std::uint16_t width {1920};
  std::uint16_t height {1080};

  bool operator==(Resolution const& other) const = default;
};


} // namespace rf3d
//

template<>
struct std::formatter<rf3d::Resolution> {
  static constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  static auto format(rf3d::Resolution const& r, std::format_context& ctx) {
    return std::format_to(ctx.out(), "{}x{}", r.width, r.height);
  }
};
