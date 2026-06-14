/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_reflection.cpp
 * @date 07/05/2026
 * @brief Test shader reflection
 *
 */

// --- Includes ---

// --- Dependencies ---
#include <mono/ranges/enumerate.hpp>
#include <reflect3d/graphics/core/pso_descriptor.hpp>
#include <reflect3d/graphics/core/pso_states.hpp>
#include <reflect3d/graphics/core/shader/compiler.hpp>
#include <reflect3d/graphics/core/shader/reflection.hpp>
#include <reflect3d/graphics/core/shader/types.hpp>
#include <reflect3d/math/math.hpp>

// --- External dependencies ---

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <slang.h>

// --- STD ---
#include <filesystem>

// --- System ---


// -------------------------------------------
// -------------------------------------------
namespace {

struct Vertex {
  rf3d::math::vec2 position {0.0F, 0.0F};
  rf3d::math::vec3 color {0.4F, 0.4F, 0.4F};
};

struct Vertex3 {
  rf3d::math::vec3 position {};
  rf3d::math::vec4 color {};
};

} // namespace

TEST_SUITE_BEGIN("Shader");

TEST_CASE("Reflection exposes all global parameters with correct names") {
  using namespace rf3d::shader;

  std::filesystem::path const shader_file =
      std::filesystem::path {mono::assets_path} / "shaders/test_bindless_shader.slang";
  auto const program = compile<SpirV>(shader_file).value();

  CHECK_FALSE(program.bytecode().empty());

  auto parameters = program.parameters();
  CHECK(std::ranges::distance(parameters) == 6);
  for (auto [idx, parameter]: parameters | mono::views::enumerate) {
    LOG_INFO("Parameter {}: name='{}', type='{}'", idx, parameter->getName(), parameter->getType()->getName());
    CHECK(std::string {parameter->getName()} == std::format("gParameter{}", idx));
  }
}

TEST_CASE("Reflection exposes all entry points with correct names and stages") {
  using namespace rf3d::shader;

  std::filesystem::path const shader_file =
      std::filesystem::path {mono::assets_path} / "shaders/test_bindless_shader.slang";
  auto const program = compile<SpirV>(shader_file).value();

  auto entry_points = program.entry_points();
  CHECK(std::ranges::distance(entry_points) == 2);
  for (auto [idx, entry_point]: entry_points | mono::views::enumerate) {
    LOG_INFO("Entry point {}: name='{}'", idx, entry_point->getName());
    CHECK(
        std::string {entry_point->getName()} ==
        std::format("{}Main", entry_point->getStage() == SLANG_STAGE_VERTEX ? "vert" : "frag")
    );
  }
}

TEST_CASE("Reflection correctly reports vertex buffer bindings") {
  std::filesystem::path const shader_file = std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang";
  auto const program = rf3d::shader::compile<rf3d::shader::SpirV>(shader_file, rf3d::shader::Stage::vertex).value();

  rf3d::VertexBufferBinding expected_binding = {
    .byte_stride = sizeof(Vertex3),
    .attributes  = {
      {.offset = offsetof(Vertex3, position), .format = rf3d::Format::rgb32_sfloat},
      {.offset = offsetof(Vertex3, color), .format = rf3d::Format::rgba32_sfloat},
    },
  };

  auto bindings = rf3d::shader::find_vertex_bindings(program);
  CHECK(std::ranges::distance(bindings) == 1);
  CHECK(bindings.at(0) == expected_binding);
}

TEST_SUITE_END();

// ============================================================
// Vertex binding reflection — format coverage and edge cases
// ============================================================

TEST_SUITE_BEGIN("VertexBindingReflection");

TEST_CASE("Float formats: float32 and float16 map to correct rf3d formats") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(
          fs::path {mono::assets_path} / "shaders/reflect_float_formats.slang", rf3d::shader::Stage::vertex
      )
          .value();

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  REQUIRE(bindings.size() == 1);

  auto const& [byte_stride, attributes] = bindings[0];
  CHECK(byte_stride == 60);
  REQUIRE(attributes.size() == 8);
  CHECK(attributes[0] == rf3d::VertexBindingAttribute {.offset = 0, .format = rf3d::Format::r32_sfloat});
  CHECK(attributes[1] == rf3d::VertexBindingAttribute {.offset = 4, .format = rf3d::Format::rg32_sfloat});
  CHECK(attributes[2] == rf3d::VertexBindingAttribute {.offset = 12, .format = rf3d::Format::rgb32_sfloat});
  CHECK(attributes[3] == rf3d::VertexBindingAttribute {.offset = 24, .format = rf3d::Format::rgba32_sfloat});
  CHECK(attributes[4] == rf3d::VertexBindingAttribute {.offset = 40, .format = rf3d::Format::r16_sfloat});
  CHECK(attributes[5] == rf3d::VertexBindingAttribute {.offset = 42, .format = rf3d::Format::rg16_sfloat});
  CHECK(attributes[6] == rf3d::VertexBindingAttribute {.offset = 46, .format = rf3d::Format::rgb16_sfloat});
  CHECK(attributes[7] == rf3d::VertexBindingAttribute {.offset = 52, .format = rf3d::Format::rgba16_sfloat});
}

TEST_CASE("Integer formats: uint32 int32 uint16 int16 map to correct rf3d formats") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(
          fs::path {mono::assets_path} / "shaders/reflect_int_formats.slang", rf3d::shader::Stage::vertex
      )
          .value();

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  REQUIRE(bindings.size() == 1);

  auto const& b = bindings[0];
  CHECK(b.byte_stride == 108);
  REQUIRE(b.attributes.size() == 14);
  CHECK(b.attributes[0] == rf3d::VertexBindingAttribute {.offset = 0, .format = rf3d::Format::r32_uint});
  CHECK(b.attributes[1] == rf3d::VertexBindingAttribute {.offset = 4, .format = rf3d::Format::rg32_uint});
  CHECK(b.attributes[2] == rf3d::VertexBindingAttribute {.offset = 12, .format = rf3d::Format::rgb32_uint});
  CHECK(b.attributes[3] == rf3d::VertexBindingAttribute {.offset = 24, .format = rf3d::Format::rgba32_uint});
  CHECK(b.attributes[4] == rf3d::VertexBindingAttribute {.offset = 40, .format = rf3d::Format::r32_sint});
  CHECK(b.attributes[5] == rf3d::VertexBindingAttribute {.offset = 44, .format = rf3d::Format::rg32_sint});
  CHECK(b.attributes[6] == rf3d::VertexBindingAttribute {.offset = 52, .format = rf3d::Format::rgb32_sint});
  CHECK(b.attributes[7] == rf3d::VertexBindingAttribute {.offset = 64, .format = rf3d::Format::rgba32_sint});
  CHECK(b.attributes[8] == rf3d::VertexBindingAttribute {.offset = 80, .format = rf3d::Format::r16_uint});
  CHECK(b.attributes[9] == rf3d::VertexBindingAttribute {.offset = 82, .format = rf3d::Format::rg16_uint});
  CHECK(b.attributes[10] == rf3d::VertexBindingAttribute {.offset = 86, .format = rf3d::Format::rgba16_uint});
  CHECK(b.attributes[11] == rf3d::VertexBindingAttribute {.offset = 94, .format = rf3d::Format::r16_sint});
  CHECK(b.attributes[12] == rf3d::VertexBindingAttribute {.offset = 96, .format = rf3d::Format::rg16_sint});
  CHECK(b.attributes[13] == rf3d::VertexBindingAttribute {.offset = 100, .format = rf3d::Format::rgba16_sint});
}

TEST_CASE("Multiple struct params produce one binding each") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(
          fs::path {mono::assets_path} / "shaders/reflect_multi_binding.slang", rf3d::shader::Stage::vertex
      )
          .value();

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  REQUIRE(bindings.size() == 2);

  // binding 0: per-vertex (position + normal + uv)
  auto const& per_vertex = bindings[0];
  CHECK(per_vertex.byte_stride == 32);
  REQUIRE(per_vertex.attributes.size() == 3);
  CHECK(per_vertex.attributes[0] == rf3d::VertexBindingAttribute {.offset = 0, .format = rf3d::Format::rgb32_sfloat});
  CHECK(per_vertex.attributes[1] == rf3d::VertexBindingAttribute {.offset = 12, .format = rf3d::Format::rgb32_sfloat});
  CHECK(per_vertex.attributes[2] == rf3d::VertexBindingAttribute {.offset = 24, .format = rf3d::Format::rg32_sfloat});

  // binding 1: per-instance (4 transform rows + tint)
  auto const& per_instance = bindings[1];
  CHECK(per_instance.byte_stride == 80);
  REQUIRE(per_instance.attributes.size() == 5);
  CHECK(
      per_instance.attributes[0] == rf3d::VertexBindingAttribute {.offset = 0, .format = rf3d::Format::rgba32_sfloat}
  );
  CHECK(
      per_instance.attributes[1] == rf3d::VertexBindingAttribute {.offset = 16, .format = rf3d::Format::rgba32_sfloat}
  );
  CHECK(
      per_instance.attributes[2] == rf3d::VertexBindingAttribute {.offset = 32, .format = rf3d::Format::rgba32_sfloat}
  );
  CHECK(
      per_instance.attributes[3] == rf3d::VertexBindingAttribute {.offset = 48, .format = rf3d::Format::rgba32_sfloat}
  );
  CHECK(
      per_instance.attributes[4] == rf3d::VertexBindingAttribute {.offset = 64, .format = rf3d::Format::rgba32_sfloat}
  );
}

TEST_CASE("Vertex pulling shader produces no vertex buffer bindings") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(
          fs::path {mono::assets_path} / "shaders/reflect_vertex_pulling.slang", rf3d::shader::Stage::vertex
      )
          .value();

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  CHECK(bindings.empty());
}

TEST_CASE("Uniform entry point params do not produce vertex buffer bindings") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(
          fs::path {mono::assets_path} / "shaders/reflect_uniforms_and_vertex.slang", rf3d::shader::Stage::vertex
      )
          .value();

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  REQUIRE(bindings.size() == 1);

  auto const& b = bindings[0];
  CHECK(b.byte_stride == 20);
  REQUIRE(b.attributes.size() == 2);
  CHECK(b.attributes[0] == rf3d::VertexBindingAttribute {.offset = 0, .format = rf3d::Format::rgb32_sfloat});
  CHECK(b.attributes[1] == rf3d::VertexBindingAttribute {.offset = 12, .format = rf3d::Format::rg32_sfloat});
}

TEST_CASE("SV_ fields inside vertex input struct are skipped") {
  namespace fs = std::filesystem;
  auto const program =
      rf3d::shader::compile<rf3d::shader::SpirV>(fs::path {mono::assets_path} / "shaders/reflect_edge_cases.slang")
          .value();

  rf3d::VertexBufferBinding const expected = {
    .byte_stride = 20,
    .attributes  = {
      {.offset = 0, .format = rf3d::Format::rgb32_sfloat},
      {.offset = 12, .format = rf3d::Format::rg32_sfloat},
    },
  };

  auto const bindings = rf3d::shader::find_vertex_bindings(program);
  // Two vertex entry points: vertSvMixed and vertUnknownFmt, one binding each
  REQUIRE(bindings.size() == 2);
  CHECK(bindings[0] == expected);
  CHECK(bindings[1] == expected);
}


TEST_SUITE_END();
