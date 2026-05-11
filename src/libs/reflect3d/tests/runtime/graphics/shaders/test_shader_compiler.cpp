
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <reflect3d/graphics/core/shader/compiler.hpp>

TEST_SUITE_BEGIN("Shaders");

TEST_CASE("Compile entire module") {
  using namespace rf3d::shader;

  auto const shader  = compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/hardcoded_triangle.slang");
  auto const shader2 = compile<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang");


  CHECK(shader.has_value());
  CHECK(shader2.has_value());
  CHECK_FALSE(shader->bytecode().empty());
  CHECK_FALSE(shader2->bytecode().empty());
}

TEST_SUITE_END();
