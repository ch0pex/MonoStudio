#pragma once

#include <cmath>
#include <cstdint>
#include <string>

namespace rf3d {

/*
 * This enum values are chosen to be compatible with
 * Vulkan's VkPrimitiveTopology, but this is not a requirement for the API.
 */
enum class PrimitiveTopology : std::uint8_t {
  point_list     = 0,
  line_list      = 1,
  line_strip     = 2,
  triangle_list  = 3,
  triangle_strip = 4,
  triangle_fan   = 5
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
  counter_clockwise,
  clockwise,
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
  solid,
  wireframe,
  point,
};

enum class DepthMode : std::uint8_t {
  none,
  read,
  write,
  read_write,
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
  FillMode fill_mode            = FillMode::solid;
  CullMode cull_mode            = CullMode::back;
  FrontFace front_face          = FrontFace::counter_clockwise;
  DepthMode depth_mode          = DepthMode::none;
  ComparisonFunction depth_test = ComparisonFunction::less_equal;
  PrimitiveTopology topology    = PrimitiveTopology::triangle_list;
};

} // namespace rf3d
