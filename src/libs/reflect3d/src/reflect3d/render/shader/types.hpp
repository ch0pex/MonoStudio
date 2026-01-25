#pragma once

#include <span>
#include <string>

namespace rf3d::gfx::shader {

using Bytecode = std::span<char const>;

enum class Stage : std::uint8_t {
  vertex = 0,
  fragment,
  compute,
  tessellation_control,
  tessellation_evaluation,
  geometry,
};

inline std::string entry_point_name(Stage const stage) {
  switch (stage) {
    case Stage::vertex:
      return "vertMain";
    case Stage::fragment:
      return "fragMain";
    case Stage::compute:
      return "computeMain";
    case Stage::tessellation_control:
      return "tessControlMain";
    case Stage::tessellation_evaluation:
      return "tessEvalMain";
    case Stage::geometry:
      return "geometryMain";
    default:
      return "main";
  }
}

} // namespace rf3d::gfx::shader
