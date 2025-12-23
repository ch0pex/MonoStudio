#include <reflect3d/graphics/vk/vk_instance.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Create instance") {
  rf3d::hri::InstanceConfig config {};
  CHECK_NOTHROW({ rf3d::hri::vk::BaseInstance instance {config}; });
}

TEST_SUITE_END();
