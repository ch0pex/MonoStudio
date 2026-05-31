
#include "reflect3d/graphics/core/shader/types.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <reflect3d/graphics/core/shader/compiler.hpp>

TEST_SUITE_BEGIN("Shaders");

TEST_CASE("compile<MultiEntryPointTarget> compiles all entry points into a single program") {
  using namespace rf3d::shader;

  auto const shader  = compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/hardcoded_triangle.slang");
  auto const shader2 = compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang");

  REQUIRE(shader.has_value());
  REQUIRE(shader2.has_value());
  CHECK_FALSE(shader->bytecode().empty());
  CHECK_FALSE(shader2->bytecode().empty());
  CHECK(std::ranges::distance(shader->entry_points()) == 2);
  CHECK(std::ranges::distance(shader2->entry_points()) == 2);
}

TEST_CASE("compile<ShaderTarget>(stage) compiles a single pipeline stage") {
  using namespace rf3d::shader;

  auto const vert =
      compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang", Stage::vertex);
  auto const frag =
      compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang", Stage::fragment);

  REQUIRE(vert.has_value());
  REQUIRE(frag.has_value());
  CHECK_FALSE(vert->bytecode().empty());
  CHECK_FALSE(frag->bytecode().empty());
  CHECK(std::ranges::distance(vert->entry_points()) == 1);
  CHECK(std::ranges::distance(frag->entry_points()) == 1);
}

TEST_CASE("compile<ShaderTarget>(stage) fails for a non-existent entry point") {
  using namespace rf3d::shader;

  auto const result =
      compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/hardcoded_triangle.slang", Stage::compute);

  CHECK_FALSE(result.has_value());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("compile<GLSL> produces one program per entry point") {
  using namespace rf3d::shader;
  std::filesystem::path const shader_path = std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang";

  auto const programs = compile<GLSL>(shader_path);

  REQUIRE(programs.has_value());
  CHECK(programs->size() == 2);
  for (auto const& program: *programs) {
    CHECK_FALSE(program.bytecode().empty());
    CHECK(std::ranges::distance(program.entry_points()) == 1);
  }
}

TEST_CASE("compile<GLSL>(stage) compiles a single pipeline stage") {
  using namespace rf3d::shader;
  std::filesystem::path const shader_path = std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang";

  auto const vert = compile<GLSL>(shader_path, Stage::vertex);
  auto const frag = compile<GLSL>(shader_path, Stage::fragment);

  REQUIRE(vert.has_value());
  REQUIRE(frag.has_value());
  CHECK_FALSE(vert->bytecode().empty());
  CHECK_FALSE(frag->bytecode().empty());
  CHECK(std::ranges::distance(vert->entry_points()) == 1);
  CHECK(std::ranges::distance(frag->entry_points()) == 1);
}

TEST_CASE("compile<HLSL>(stage) compiles a single pipeline stage") {
  using namespace rf3d::shader;
  std::filesystem::path const shader_path = std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang";

  auto const vert = compile<HLSL>(shader_path, Stage::vertex);
  auto const frag = compile<HLSL>(shader_path, Stage::fragment);

  REQUIRE(vert.has_value());
  REQUIRE(frag.has_value());
  CHECK_FALSE(vert->bytecode().empty());
  CHECK_FALSE(frag->bytecode().empty());
}

TEST_CASE("compile<WGSL>(stage) compiles a single pipeline stage") {
  using namespace rf3d::shader;
  std::filesystem::path const shader_path = std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang";

  auto const vert = compile<WGSL>(shader_path, Stage::vertex);
  auto const frag = compile<WGSL>(shader_path, Stage::fragment);

  REQUIRE(vert.has_value());
  REQUIRE(frag.has_value());
  CHECK_FALSE(vert->bytecode().empty());
  CHECK_FALSE(frag->bytecode().empty());
}

TEST_SUITE_END();
