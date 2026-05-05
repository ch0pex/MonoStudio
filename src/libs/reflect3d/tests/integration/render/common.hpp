/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file common.hpp
 * @date 13/04/2026
 * @brief Short description
 */

#pragma once

#include <reflect3d/graphics/rhi.hpp>

namespace test {

struct Vertex {
  rf3d::math::vec3 position {0.0F, 0.0F, 0.0F};
  rf3d::math::vec4 color {0.4F, 0.4F, 0.4F, 0.0F};
};

namespace triangle {

inline constexpr Vertex vertex1 {.position = {0.0F, -0.5F, 0.0F}, .color = {1.0F, 0.0F, 0.0F, 0.0F}};
inline constexpr Vertex vertex2 {.position = {0.5F, 0.5F, 0.0F}, .color = {0.0F, 1.0F, 0.0F, 0.0F}};
inline constexpr Vertex vertex3 {.position = {-0.5F, 0.5F, 0.0F}, .color = {0.0F, 0.0F, 1.0F, 0.0F}};
inline constexpr std::array vertices                  = {vertex1, vertex2, vertex3};
inline constexpr std::array<std::uint16_t, 3> indices = {0, 1, 2};

} // namespace triangle

namespace square {

std::array constexpr vertices = {
  Vertex {.position = {-0.5F, -0.5F, 0.F}, .color = {1.0F, 0.0F, 0.0F, 0.0F}},
  Vertex {.position = {0.5F, -0.5F, 0.F}, .color = {0.0F, 1.0F, 0.0F, 0.F}},
  Vertex {.position = {0.5F, 0.5F, 0.F}, .color = {0.0F, 0.0F, 1.0F, 0.F}},
  Vertex {.position = {-0.5F, 0.5F, 0.F}, .color = {1.0F, 1.0F, 1.0F, 0.F}}
};
inline constexpr std::array<std::uint16_t, 6> indices = {0, 1, 2, 2, 3, 0};

} // namespace square

inline auto const shader_path     = std::filesystem::path {mono::assets_path} / "shaders" / "basic_shader.slang";
inline auto const shader_bytecode = rf3d::shader::compile_module<rf3d::shader::SpirV>(shader_path);
template<rf3d::RenderHardwareInterface Rhi = rf3d::impl::DefaultBackend>
rf3d::rhi::pipeline_state_t<Rhi> create_basic_pso() {
  return rf3d::rhi::pipeline_state_t<Rhi> {
    rf3d::PipelineCreateInfo {
      .debug_name             = "Triangle PSO",
      .vertex_shader          = {.bytecode = shader_bytecode},
      .fragment_shader        = {.bytecode = shader_bytecode},
      .rasterizer_state       = {.cull_mode = rf3d::CullMode::none},
      .vertex_buffer_bindings = {
        {
          .byte_stride = sizeof(Vertex),
          .attributes  = {
            {.offset = offsetof(Vertex, position), .format = rf3d::Format::rgb32_sfloat},
            {.offset = offsetof(Vertex, color), .format = rf3d::Format::rgba32_sfloat},
          },
        },
      },
    },
  };
}

} // namespace test
