#include <rflect3d/graphics/vk/vk_extension.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("One window") {
  rflect::gfx::vk::log_extensions();
  CHECK(rflect::gfx::vk::extensions().size() > 0);
}


TEST_SUITE_END();
