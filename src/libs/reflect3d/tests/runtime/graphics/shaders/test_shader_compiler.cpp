
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <reflect3d/graphics/core/shader/compiler.hpp>

TEST_SUITE_BEGIN("Shaders");

TEST_CASE("Compile entire module") {
  using namespace rf3d::shader;

  auto const bytecode =
      compile_module<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/hardcoded_triangle.slang");

  auto const bytecode2 =
      compile_module<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/basic_shader.slang");
  CHECK_FALSE(bytecode.empty());
  CHECK_FALSE(bytecode2.empty());
}

TEST_SUITE_END();
