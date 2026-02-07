
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <reflect3d/render/shader/compiler.hpp>

TEST_SUITE_BEGIN("Shaders");

TEST_CASE("Compile entire module") {
  using namespace rf3d::gfx::shader;

  auto const bytecode =
      compile_module<SpirV>(std::filesystem::path {mono::assets_path} / "shaders/hardcoded_triangle.slang");
  CHECK_FALSE(bytecode.empty());
}

TEST_SUITE_END();
