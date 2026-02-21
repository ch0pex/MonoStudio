#pragma once

#include "reflect3d/window/utils/resolution.hpp"

#include <format>


template<>
struct std::formatter<rf3d::Resolution> {
  static constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  static auto format(rf3d::Resolution const& r, std::format_context& ctx) {
    return std::format_to(ctx.out(), "{}x{}", r.width, r.height);
  }
};
