#pragma once

#include <cmath>
#include <cstdint>

namespace rf3d::gfx {

enum class PrimitiveTopology : std::uint8_t {
  point_list,
  line_list,
  line_strip,
  triangle_list,
  triangle_strip,
  triangle_fan,
};

enum class ComparisonFunction : std::uint8_t {
  never,
  less,
  equal,
  less_equal,
  greater,
  not_equal,
  greater_equal,
  always,
};

enum class FrontFace : std::uint8_t {
  clockwise,
  counter_clockwise,
};

enum class CullMode : std::uint8_t {
  none,
  front,
  back,
};

enum class PolygonMode : std::uint8_t {
  fill,
  line,
  point,
};

enum class FillMode : std::uint8_t {
  wireframe,
  solid,
};

enum class Blend : std::uint8_t {

};

enum class BlendOperation : std::uint8_t {

};

enum class PsoType : std::uint8_t {
  graphics,
  compute,
  mesh_shader,
};

struct RasterizerState {
  FillMode fill_mode                     = FillMode::solid;
  CullMode cull_mode                     = CullMode::back;
  FrontFace front_face                   = FrontFace::counter_clockwise;
  std::int32_t depth_bias                = 0;
  float depth_bias_clamp                 = 0.0F;
  float slope_scaled_depth_bias          = 0.0F;
  bool depth_clip_enable                 = true;
  bool multisample_enable                = false;
  bool antialiased_line_enable           = false;
  bool conservative_rasterization_enable = false;
  std::uint32_t forced_sample_count      = 0;
};

} // namespace rf3d::gfx
