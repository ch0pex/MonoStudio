#include <reflect3d/graphics/vk/vk_instance.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Create instance") {
  CHECK_NOTHROW({ rf3d::gfx::vk::Instance instance {}; });
}

TEST_CASE("Create two instances") {
  CHECK_NOTHROW({
    rf3d::gfx::vk::Instance instance {};
    rf3d::gfx::vk::Instance instance2 {};
  });
}

TEST_SUITE_END();
